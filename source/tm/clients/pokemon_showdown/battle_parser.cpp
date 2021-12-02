// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/end_of_turn_state.hpp>
#include <tm/clients/pokemon_showdown/move_state.hpp>
#include <tm/clients/pokemon_showdown/slot_memory.hpp>

#include <tm/clients/log_foe_team.hpp>

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/team.hpp>
#include <tm/string_conversions/weather.hpp>

#include <tm/ability_blocks_move.hpp>
#include <tm/battle.hpp>
#include <tm/generation_generic.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>
#include <bounded/detail/overload.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/begin_end.hpp>
#include <containers/index_type.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {

using namespace std::string_view_literals;

BattleParser::~BattleParser() = default;

namespace {

constexpr auto party_from_player_id(std::string_view const player_id) {
	return parse_identity(player_id).party;
}

auto get_move_index(RegularMoves const moves, Moves const move_name) {
	auto const it = containers::find_if(moves, [=](Move const move) { return move.name() == move_name; });
	if (it == containers::end(moves)) {
		throw std::runtime_error(containers::concatenate<std::string>("Pokemon does not know "sv, to_string(move_name)));
	}
	return bounded::assume_in_range<containers::index_type<RegularMoves>>(it - containers::begin(moves));
}

constexpr auto to_normal_weather(Weather const weather) {
	if (weather.hail()) {
		return NormalWeather::hail;
	} else if (weather.sand()) {
		return NormalWeather::sand;
	} else if (weather.sun()) {
		return NormalWeather::sun;
	} else if (weather.rain()) {
		return NormalWeather::rain;
	} else {
		return NormalWeather::clear;
	}
}

constexpr auto parse_status(std::string_view const str) {
	return
		(str == "") ? Statuses::clear :
		(str == "fnt") ? Statuses::clear :
		(str == "brn") ? Statuses::burn :
		(str == "frz") ? Statuses::freeze :
		(str == "par") ? Statuses::paralysis :
		(str == "psn") ? Statuses::poison :
		(str == "tox") ? Statuses::toxic :
		(str == "slp") ? Statuses::sleep :
//		(str == "") ? Statuses::rest :
		throw std::runtime_error("Received an invalid status");
}

constexpr auto parse_hp_and_status(std::string_view const hp_and_status) {
	auto const [hp_fraction, status] = split_view(hp_and_status, ' ');
	return HPAndStatus{parse_hp(hp_fraction), parse_status(status)};
}

struct FromMove{};
struct MainEffect{};
struct FromConfusion{};
struct FromMiscellaneous{};
struct FromRecoil{};
struct FromSubstitute{};

using EffectSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous, FromRecoil, FromSubstitute>;

constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) {
	return
		(type == "") ? EffectSource(MainEffect{}) :
		(type == "item") ? EffectSource(from_string<Item>(source)) :
		(type == "ability") ? EffectSource(from_string<Ability>(source)) :
		(type == "move") ? EffectSource(FromMove{}) :
		(type == "confusion") ? EffectSource(FromConfusion{}) :
		(type == "Recoil") ? EffectSource(FromRecoil{}) :
		(type == "Substitute") ? EffectSource(FromSubstitute{}) :
		(
			type == "brn" or
			type == "psn" or
			type == "tox" or
			type == "drain" or
			type == "perish3" or
			type == "perish2" or
			type == "perish1" or
			type == "perish0" or
			type == "trapped" or
			type == "Encore" or
			type == "Hail" or
			type == "Leech Seed" or
			type == "Light Screen" or
			type == "Protect" or // Includes Detect
			type == "Spikes" or
			type == "Stealth Rock" or
			type == "Reflect" or
			type == "Sandstorm"
		) ? EffectSource(FromMiscellaneous{}) :
		throw std::runtime_error(containers::concatenate<std::string>(std::string_view("Unhandled effect source type: "), type));
}

auto parse_from_source(InMessage message) {
	[[maybe_unused]] auto const from_or_silent_or_nothing = message.pop(' ');
	auto const [type, source] = split_view(message.pop(), ':');
	return parse_effect_source(type, source);
}

auto parse_hp_message(InMessage message) {
	struct Message {
		Party party;
		VisibleHP hp;
		Statuses status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message);
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

auto parse_set_hp_message(InMessage message) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct Message {
		Party party;
		VisibleHP hp;
		Statuses status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message);
	
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

auto hp_to_damage(any_pokemon auto const & pokemon, HP::current_type const new_hp) {
	auto const old_hp = pokemon.hp().current();
	if (new_hp > old_hp) {
		std::cerr << "Took negative damage\n";
		return HP::current_type(0_bi);
		// throw std::runtime_error("Took negative damage");
	}
	return bounded::assume_in_range<HP::current_type>(old_hp - new_hp);
}

auto const & select_pokemon(any_team auto const & team, Moves const move) {
	return is_switch(move) ? team.pokemon(to_replacement(move)) : team.all_pokemon()();
}

constexpr auto causes_recoil(Moves const move) {
	switch (move) {
		case Moves::Brave_Bird:
		case Moves::Double_Edge:
		case Moves::Flare_Blitz:
		case Moves::Head_Smash:
		case Moves::Submission:
		case Moves::Take_Down:
		case Moves::Volt_Tackle:
		case Moves::Wood_Hammer:
			return true;
		default:
			return false;
	}
}

constexpr auto cures_target_status(Generation const generation, Moves const move_name, bounded::optional<Type> const hidden_power_type, Statuses const status) {
	switch (status) {
		case Statuses::freeze:
			return
				get_type(generation, move_name, hidden_power_type) == Type::Fire or
				(generation == Generation::two and move_name == Moves::Tri_Attack);
		case Statuses::paralysis:
			return move_name == Moves::Smelling_Salts;
		case Statuses::rest:
		case Statuses::sleep:
			return move_name == Moves::Wake_Up_Slap;
		case Statuses::clear:
		case Statuses::burn:
		case Statuses::poison:
		case Statuses::toxic:
			return false;
	}
}

constexpr auto moved(any_team auto const & team) -> bool {
	auto const pokemon = team.pokemon();
	return pokemon.last_used_move().moved_this_turn() or pokemon.hp().current() == 0_bi;
};

template<Generation generation>
struct BattleParserImpl : BattleParser {
	BattleParserImpl(
		SendMessageFunction send_message,
		BattleLogger battle_logger,
		std::ofstream analysis_logger,
		containers::string id_,
		containers::string username,
		UsageStats const & usage_stats,
		Evaluate<generation> evaluate,
		Party party,
		DepthValues const depth,
		std::mt19937 random_engine,
		KnownTeam<generation> ai,
		SeenTeam<generation> foe,
		bool log_foe_teams
	):
		m_usage_stats(usage_stats),
		m_send_message(std::move(send_message)),
		m_battle_logger(std::move(battle_logger)),
		m_analysis_logger(std::move(analysis_logger)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_random_engine(random_engine),
		m_slot_memory(ai.size()),
		m_evaluate(evaluate),
		m_battle(
			std::move(ai),
			std::move(foe)
		),
		m_ai_party(party),
		m_depth(depth),
		m_log_foe_teams(log_foe_teams)
	{
		m_send_message(containers::concatenate<containers::string>(m_id, "|/timer on"sv));
	}
	
	void handle_message(InMessage message) final {
		m_battle_logger.log(message);
		
		if (handle_chat_message(message)) {
			return;
		}

		auto const type = message.type();
		if (type.empty() or (type.front() != '-' and type != "drag" and type != "move" and type != "switch")) {
			maybe_use_previous_move();
		}

		// Documented at
		// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
		// under the section "Battle progress"
		if (type == "") {
			// Do nothing. This message is used to indicate the end of a series.
		} else if (type == "-ability") {
			auto const party = party_from_player_id(message.pop());
			auto const ability = from_string<Ability>(message.pop());
			maybe_commit_switch(party);
			set_value_on_pokemon(party, ability);
		} else if (type == "-activate") {
			auto const party = party_from_player_id(message.pop());
			auto const [category, source] = split_view(message.pop(), ": "sv);
			auto const details = message.pop();
			maybe_commit_switch(party);
			bounded::visit(parse_effect_source(category, source), bounded::overload(
				[](MainEffect) { throw std::runtime_error("Unexpected -activate source MainEffect"); },
				[](FromConfusion) {},
				[](FromMiscellaneous) {},
				[](FromMove) {},
				[](FromRecoil) { throw std::runtime_error("Unexpected -activate source FromRecoil"); },
				[&](FromSubstitute) {
					if (details == "[damage]") {
						m_move_state.damage_substitute(other(party));
						// TODO: Why
						handle_u_turn(other(party));
					}
				},
				[&](Ability const ability) {
					set_value_on_pokemon(party, ability);
					switch (ability) {
						case Ability::Forewarn:
							m_battle.add_move(!is_ai(party), from_string<Moves>(details));
							break;
						case Ability::Shed_Skin:
							m_end_of_turn_state.shed_skin(party);
							break;
						default:
							break;
					}
				},
				[&](Item const item) { set_value_on_pokemon(party, item); }
			));
		} else if (type == "-anim") {
#if 0
			auto const party = party_from_player_id(message.pop());
			// Solar Beam is the only example I have of this
			auto const move? = message.pop();
			auto const target? = message.pop();
#endif
		} else if (type == "-boost") {
#if 0
			auto const pokemon = message.pop();
			auto const stat = message.pop();
			auto const amount = message.pop();
#endif
		} else if (type == "cant") {
			// TODO: Figure out how to solve this in general...
			auto const party = party_from_player_id(message.pop());
			auto const reason = message.pop();
			auto use_move = [&](Moves const move_name) {
				m_move_state.use_move(party, move_name);
			};
			if (reason == "flinch") {
				// Technically incorrect with things like Sucker Punch and
				// priority
				use_move(Moves::Struggle);
				m_move_state.flinch(party);
			} else if (reason == "frz") {
				// Using a move like Flame Wheel does not send "cant"
				// Technically incorrect with things like Sucker Punch and
				// priority
				use_move(Moves::Struggle);
			} else if (reason == "par") {
				// Technically incorrect with things like Sucker Punch and
				// priority
				use_move(Moves::Struggle);
				m_move_state.fully_paralyze(party);
			} else if (reason == "slp") {
				m_move_state.still_asleep(party);
			} else if (reason == "recharge") {
				apply_to_team(is_ai(party), [&](auto const & team) {
					use_move(team.pokemon().last_used_move().name());
				});
			} else {
				std::cerr << "Received unknown \"cant\" reason: " << reason;
				if (!message.remainder().empty()) {
					std::cerr << '|' << message.remainder();
				}
				std::cerr << '\n';
			}
		} else if (type == "-center") {
		} else if (type == "-clearallboost") {
			// We already know what Haze does
		} else if (type == "-crit") {
			auto const user = other(party_from_player_id(message.pop()));
			m_move_state.critical_hit(user);
		} else if (type == "-curestatus") {
			auto const party = party_from_player_id(message.pop());
			if (moved(m_battle.ai()) and moved(m_battle.foe())) {
				m_end_of_turn_state.set_expected(party, Statuses::clear);
			} else if (!m_move_state.party() or *m_move_state.party() == party) {
				m_move_state.clear_status(party);
			} else {
				auto try_use_previous_move = [&] {
					auto const move_name = m_move_state.executed_move();
					if (!move_name) {
						return true;
					}
					return apply_to_team(is_ai(party), [&](auto & team) {
						auto const pokemon = team.pokemon();
						return !cures_target_status(
							generation,
							*move_name,
							get_hidden_power_type(pokemon),
							pokemon.status().name()
						);
					});
				};
				if (try_use_previous_move()) {
					maybe_use_previous_move();
				}
				m_move_state.clear_status(party);
				if constexpr (generation == Generation::one) {
					// TODO: Try to do something smarter here
					m_move_state.use_move(party, Moves::Struggle);
				}
			}
			auto const source = parse_from_source(message);
			bounded::visit(source, bounded::overload(
				[&](Ability const ability) { set_value_on_pokemon(party, ability); },
				[](auto) { }
			));
		} else if (type == "-cureteam") {
#if 0
			auto const pokemon = message.pop();
#endif
		} else if (type == "-damage") {
			handle_damage(message);
		} else if (type == "detailschange" or type == "-formechange") {
#if 0
			auto const pokemon = message.pop();
			// message.remainder() == DETAILS|HP STATUS
#endif
		} else if (type == "-end") {
			// Tells me if the substitute is destroyed, probably other stuff ending
			auto const party = party_from_player_id(message.pop());
			auto const source = message.pop();
			if (source == "Substitute") {
				m_move_state.break_substitute(other(party));
				handle_u_turn(other(party));
			}
		} else if (type == "-endability") {
#if 0
			auto const pokemon = message.pop();
			auto const ability = message.pop();
#endif
		} else if (type == "-enditem") {
			auto const party = party_from_player_id(message.pop());
			auto const item = from_string<Item>(message.pop());
			set_value_on_pokemon(party, item);
		} else if (type == "error") {
			if (message.remainder() != "[Invalid choice] There's nothing to choose") {
				send_random_move();
			}
		} else if (type == "-fail") {
#if 0
			auto const party = party_from_player_id(message.pop());
			// There can be more to this message, but nothing we care about
#endif
		} else if (type == "faint") {
			auto const party = party_from_player_id(message.pop());
			if (generation <= Generation::three and is_ai(party) and m_battle.ai().size() != 1_bi) {
				m_replacing_fainted = true;
				send_move(determine_action());
			}
		} else if (type == "-fieldend") {
#if 0
			// Grassy Terrain, Gravity, Trick Room
			auto const condition = message.pop();
#endif
		} else if (type == "-fieldstart") {
#if 0
			// Grassy Terrain, Gravity, Trick Room
			auto const condition = message.pop();
#endif
		} else if (type == "-heal") {
			auto const parsed = parse_hp_message(message);
			auto const party = parsed.party;
			bounded::visit(parsed.source, bounded::overload(
				[](MainEffect) {},
				[](FromConfusion) { throw std::runtime_error("Confusion cannot heal"); },
				[](FromMiscellaneous) {},
				[](FromMove) {},
				[](FromRecoil) { throw std::runtime_error("Recoil cannot heal"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot heal"); },
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
			queue_hp_or_status_checks(party, parsed.hp);
			queue_hp_or_status_checks(party, parsed.status);
		} else if (type == "-hint") {
			// message.remainder() == MESSAGE
		} else if (type == "-hitcount") {
			auto const party = party_from_player_id(message.pop());
			auto const count = bounded::to_integer<2, 5>(message.pop());
			static_cast<void>(party);
			static_cast<void>(count);
			// TODO: Implement multi-hit moves
		} else if (type == "-immune") {
			auto const party = party_from_player_id(message.pop());
			auto const source = parse_from_source(message);
			bounded::visit(source, bounded::overload(
				// TODO: Validate that the type should be immune
				[](MainEffect) {},
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause immunity"); },
				[](FromMiscellaneous) { throw std::runtime_error("Miscellaneous effects cannot cause immunity"); },
				[](FromMove) { throw std::runtime_error("Moves cannot cause immunity"); },
				[](FromRecoil) { throw std::runtime_error("Recoil cannot cause immunity"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot cause immunity"); },
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
		} else if (type == "inactive") {
			// message.remainder() == MESSAGE
			// Timer is on
		} else if (type == "inactiveoff") {
			// message.remainder() == MESSAGE
			// Timer is off
		} else if (type == "-item") {
			auto const party = party_from_player_id(message.pop());
			auto const item = from_string<Item>(message.pop());
			set_value_on_pokemon(party, item);
		} else if (type == "-mega") {
#if 0
			auto const pokemon = message.pop();
			auto const megastone = message.pop();
#endif
		} else if (type == "-message") {
		} else if (type == "-miss") {
			auto const user_party = party_from_player_id(message.pop());
			m_move_state.miss(user_party);
		} else if (type == "move") {
			auto const party = party_from_player_id(message.pop());
			auto const move = from_string<Moves>(message.pop());
#if 0
			// target is sent only for moves that target one Pokemon
			auto const target = message.pop();
#endif
			if (m_move_state.party() != party) {
				maybe_use_previous_move();
			}
			m_move_state.use_move(party, move);
			if (is_ai(party) and move == Moves::Baton_Pass and m_battle.ai().size() != 1_bi) {
				maybe_use_previous_move();
				send_move(determine_action());
			}
		} else if (type == "-mustrecharge") {
			// After moves like Hyper Beam
		} else if (type == "-notarget") {
			// When you use a move, but there is no one to target
		} else if (type == "player") {
			// At the end of a battle, I received this with a body of "p1|"
		} else if (type == "-prepare") {
			// From moves like SolarBeam on the charge turn
		} else if (type == "request") {
#if 0
			auto const json = message.remainder();
#endif
		} else if (type == "-resisted") {
			// message.remainder() == POKEMON
		} else if (type == "-sethp") {
			auto const parsed = parse_set_hp_message(message);
			static_cast<void>(parsed);
		} else if (type == "-sideend") {
#if 0
			// Light Screen, Reflect, etc.
			auto const side = message.pop();
			auto const condition = message.pop();
#endif
		} else if (type == "-sidestart") {
#if 0
			// Light Screen, Reflect, etc.
			auto const side = message.pop();
			auto const condition = message.pop();
#endif
		} else if (type == "-supereffective") {
			// message.remainder() == POKEMON
		} else if (type == "switch") {
			if (!m_ai_switch and !m_foe_switch) {
				maybe_use_previous_move();
			}
			auto const parsed = handle_switch_or_drag(message);
			auto & switch_ = get_switch(parsed.party);
			if (switch_) {
				throw std::runtime_error("Tried to switch in the same Pokemon twice");
			}
			bounded::insert(switch_, Switch{parsed.move, parsed.hp, parsed.status});
		} else if (type == "drag") {
			auto const parsed = handle_switch_or_drag(message);
			apply_to_team(is_ai(parsed.party), [&](auto & team) {
				m_move_state.phaze_index(other(parsed.party), team.all_pokemon(), parsed.species);
			});
			m_move_state.set_expected(parsed.party, parsed.hp);
			m_move_state.set_expected(parsed.party, parsed.status);
		} else if (type == "replace") {
#if 0
			// Illusion ended
			auto const pokemon = message.pop();
			// message.remainder() == DETAILS|HP STATUS
#endif
		} else if (type == "-singleturn") {
			// Received for things like Protect that last the rest of the turn
		} else if (type == "-start") {
			auto const party = party_from_player_id(message.pop());
			auto const first_part_of_source = message.pop();
			auto parse_remainder_of_source = [&]{
				if (first_part_of_source == "typechange") {
					auto const changed_type = message.pop();
					static_cast<void>(changed_type);
					return parse_from_source(message);
				} else {
					auto const [source_type, string_source] = split_view(first_part_of_source, ':');
					return parse_effect_source(source_type, string_source);
				}
			};
			auto const source = parse_remainder_of_source();
			bounded::visit(source, bounded::overload(
				[](MainEffect) { throw std::runtime_error("Unexpected -start source MainEffect"); },
				[&](FromConfusion) {
					auto const how = message.pop();
					if (how == "[fatigue]") {
						m_end_of_turn_state.lock_in_ends(party);
					} else {
						m_move_state.confuse();
					}
				},
				[](FromMiscellaneous) {},
				[](FromMove) {},
				[](FromRecoil) { throw std::runtime_error("Unexpected -start source FromRecoil"); },
				[](FromSubstitute) {},
				[&](auto const value) { set_value_on_pokemon(party, value); }
			));
		} else if (type == "-status") {
			auto const party = party_from_player_id(message.pop());
			auto const status = parse_status(message.pop());
			auto const source = parse_from_source(message);
			maybe_commit_switch(party);
			bounded::visit(source, bounded::overload(
				[&](MainEffect) { m_move_state.status_from_move(party, status); },
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
				[&](FromMiscellaneous) { m_move_state.status_from_move(other(party), status); },
				[&](FromMove) { m_move_state.status_from_move(party, status); },
				[](FromRecoil) { throw std::runtime_error("Recoil cannot cause another status"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot cause another status"); },
				[&](auto const value) {
					set_value_on_pokemon(party, value);
					m_move_state.set_expected(party, status);
				}
			));
		} else if (type == "swap") {
#if 0
			auto const pokemon = message.pop();
			auto const position = message.pop();
			// position here is "0", "1", or "2"
#endif
		} else if (type == "t:") {
			// message.remainder() == Seconds since 1970
		} else if (type == "tie") {
			m_completed = true;
		} else if (type == "-transform") {
			// message.remainder() == POKEMON|SPECIES
		} else if (type == "turn") {
			auto const turn = bounded::to_integer<0, numeric_traits::max_value<std::uint32_t>>(message.pop());
			m_analysis_logger << containers::string(containers::repeat_n(20_bi, '=')) << "\nBegin turn " << turn << '\n';
			m_battle.handle_begin_turn();
			send_move(determine_action());
		} else if (type == "-unboost") {
#if 0
			auto const pokemon = message.pop();
			auto const stat = message.pop();
			auto const amount = message.pop();
#endif
		} else if (type == "upkeep") {
			auto const end_of_turn_state = m_end_of_turn_state.complete();
			bool const ai_went_first = end_of_turn_state.first_party == m_ai_party;
			m_battle.handle_end_turn(ai_went_first, end_of_turn_state.first.flags, end_of_turn_state.last.flags);
			validate_weather(end_of_turn_state.weather);
			try_correct_hp_and_status(ai_went_first, end_of_turn_state.first.hp, end_of_turn_state.first.status);
			try_correct_hp_and_status(!ai_went_first, end_of_turn_state.last.hp, end_of_turn_state.last.status);
			if (generation >= Generation::four and m_battle.ai().pokemon().hp().current() == 0_bi) {
				m_replacing_fainted = true;
				send_move(determine_action());
			}
		} else if (type == "-weather") {
			auto const weather = from_string<NormalWeather>(message.pop());
			m_end_of_turn_state.active_weather(weather);
			[[maybe_unused]] auto const from_or_upkeep_or_nothing = message.pop(' ');
			auto const category = message.pop(": "sv);
			auto const source = message.pop();
			bounded::visit(parse_effect_source(category, source), bounded::overload(
				[&](Ability const ability) {
					[[maybe_unused]] auto const of = message.pop(' ');
					auto const party = party_from_player_id(message.pop());
					maybe_commit_switch(party);
					set_value_on_pokemon(party, ability);
				},
				[](auto) { }
			));
		} else if (type == "win") {
			if (m_log_foe_teams) {
				auto const won = m_username == message.pop();
				if (!won) {
					log_foe_team(m_usage_stats, m_battle.foe(), m_random_engine);
				}
			}
			m_completed = true;
		} else {
			std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
		}
	}

	std::string_view id() const final {
		return m_id;
	}	
	bool completed() const final {
		return m_completed;
	}

private:
	auto compute_damage(bool const user_is_ai, Moves const move, HPAndStatus const hp_and_status) -> const HP::current_type {
		auto const ai_damaged = !user_is_ai xor (move == Moves::Hit_Self);
		return apply_to_team(ai_damaged, [&](auto const & team) {
			auto const & pokemon = select_pokemon(team, move);
			auto const new_hp = ai_damaged ?
				hp_and_status.hp.current.value() :
				to_real_hp(pokemon.hp().max(), hp_and_status.hp).value;
			return hp_to_damage(pokemon, new_hp);
		});
	}
	auto is_ai(Party const party) const {
		return party == m_ai_party;
	}
	decltype(auto) apply_to_team(bool const is_ai_, auto function) const {
		if (is_ai_) {
			return function(m_battle.ai());
		 } else {
			 return function(m_battle.foe());
		 }
	}

	void handle_damage(InMessage message) {
		auto const parsed = parse_hp_message(message);
		auto move_damage = [&](Party const party) {
			m_move_state.damage(party, {parsed.hp, parsed.status});
		};
		auto const party = parsed.party;
		maybe_commit_switch(party);
		bounded::visit(parsed.source, bounded::overload(
			[&](MainEffect) {
				if (m_move_state.move_damages_self(party)) {
					return;
				}
				move_damage(other(party));
				handle_u_turn(other(party));
			},
			[&](FromConfusion) {
				// TODO: Technically you cannot select Hit Self, you just execute
				// it. This matters for things like priority or determining whether
				// Sucker Punch succeeds. As a workaround for now, say the user
				// selected Struggle.
				// When hitting self in confusion, we get -activate then -damage
				maybe_use_previous_move();
				m_move_state.use_move(party, Moves::Struggle);
				m_move_state.use_move(party, Moves::Hit_Self);
				move_damage(party);
			},
			[](FromMiscellaneous) {},
			[](FromMove) {},
			[&](FromRecoil) { m_move_state.recoil(party); },
			[](FromSubstitute) {},
			[&](auto const value) { set_value_on_pokemon(party, value); }
		));
		queue_hp_or_status_checks(party, parsed.hp);
		queue_hp_or_status_checks(party, parsed.status);
	}

	auto handle_switch_or_drag(InMessage message) {
		auto parsed = parse_switch(message);
		auto const data_is_for_ai = is_ai(parsed.party);
		if (data_is_for_ai) {
			auto const index = find_required_index(m_battle.ai().all_pokemon(), parsed.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		auto const move = m_battle.find_or_add_pokemon(
			data_is_for_ai,
			parsed.species,
			std::move(parsed).nickname,
			parsed.level,
			parsed.gender
		);
		struct Result {
			Party party;
			Species species;
			Moves move;
			VisibleHP hp;
			Statuses status;
		};
		return Result{parsed.party, parsed.species, move, parsed.hp, parsed.status};
	}

	void set_value_on_pokemon(Party const party, auto const value) {
		if (auto const switch_index = m_move_state.switch_index()) {
			m_battle.set_value_on_index(is_ai(party), *switch_index, value);
		} else {
			m_battle.set_value_on_active(is_ai(party), value);
		}
	}

	void queue_hp_or_status_checks(Party const party, auto const value) {
		if (m_move_state.party()) {
			m_move_state.set_expected(party, value);
		} else {
			m_end_of_turn_state.set_expected(party, value);
		}
	}

	auto & get_switch(Party const party) {
		return is_ai(party) ? m_ai_switch : m_foe_switch;
	}

	void maybe_commit_switch(Party const party) {
		auto & switch_ = get_switch(party);
		if (!switch_) {
			return;
		}
		auto const move_party = m_move_state.party();
		if (move_party and *move_party != party) {
			maybe_use_previous_move_impl();
		}
		m_move_state.use_move(party, switch_->move);
		m_move_state.set_expected(party, switch_->hp);
		m_move_state.set_expected(party, switch_->status);
		switch_ = bounded::none;
	}
	void maybe_use_previous_move() {
		maybe_use_previous_move_impl();
		maybe_commit_switch(m_ai_party);
		maybe_commit_switch(other(m_ai_party));
		maybe_use_previous_move_impl();
	}
	void maybe_use_previous_move_impl() {
		struct LocalDamage {
			ActualDamage value;
			bool did_any_damage;
		};
		auto const maybe_data = m_move_state.complete<generation>(m_ai_party, m_battle.ai(), m_battle.foe(), m_battle.weather());

		bounded::visit(maybe_data, [&]<typename Data>(Data const data) {
			if constexpr (std::is_same_v<Data, MoveState::NoResult>) {
				return;
			} else {
				constexpr auto data_is_for_ai = std::is_same_v<Data, MoveState::AIResult<generation>>;
				m_battle.add_move(data_is_for_ai, data.move.selected);
				if (data.move.selected == Moves::Sleep_Talk) {
					m_battle.add_move(data_is_for_ai, data.move.executed);
				}
				auto const user_pokemon = [&] {
					if constexpr (data_is_for_ai) {
						return m_battle.ai().pokemon();
					} else {
						return m_battle.foe().pokemon();
					}
				}();
				auto const other_pokemon = [&] {
					if constexpr (data_is_for_ai) {
						return m_battle.foe().pokemon();
					} else {
						return m_battle.ai().pokemon();
					}
				}();
				auto const move_type = get_type(generation, data.move.executed, get_hidden_power_type(user_pokemon));
				auto const known_move = [&] {
					return KnownMove{
						data.move.executed,
						move_type
					};
				};
				auto const ability_blocks_recoil =
					causes_recoil(data.move.executed) and
					!data.recoil and
					!ability_blocks_move(
						generation,
						other_pokemon.ability(),
						known_move(),
						other_pokemon.status().name(),
						other_pokemon.types()
					);
				if (ability_blocks_recoil) {
					// TODO: This could also be Magic Guard
					m_battle.set_value_on_active(data_is_for_ai, Ability::Rock_Head);
				}
				
				auto const damage = bounded::visit(data.damage, bounded::overload(
					[](NoDamage) {
						return LocalDamage{ActualDamage::Known{0_bi}, false};
					},
					[&](HPAndStatus const hp_and_status) -> LocalDamage {
						auto const value = compute_damage(data_is_for_ai, data.move.executed, hp_and_status);
						return LocalDamage{
							ActualDamage::Known{value},
							value != 0_bi
						};
					},
					[&](SubstituteDamaged) -> LocalDamage {
						if (!other_pokemon.substitute()) {
							throw std::runtime_error("Tried to damage a Substitute when the target does not have a Substitute");
						}
						return LocalDamage{ActualDamage::Capped{bounded::increase_min<0>(other_pokemon.substitute().hp() - 1_bi)}, true};
					},
					[&](SubstituteBroke) -> LocalDamage {
						return LocalDamage{ActualDamage::Known{other_pokemon.substitute().hp()}, true};
					}
				));
				
				auto const other_move = other_pokemon.last_used_move().moved_this_turn() ?
					OtherMove([&]{
						auto const move = other_pokemon.last_used_move().name();
						auto const type = get_type(generation, move, get_hidden_power_type(other_pokemon));
						return KnownMove{move, type};
					}()) :
					OtherMove(FutureMove{
						data.move.executed == Moves::Sucker_Punch and damage.did_any_damage
					});

				m_battle.handle_use_move(data.move, data.clear_status, damage.value, other_move);

				try_correct_hp_and_status(data_is_for_ai, data.user.hp, data.user.status);
				try_correct_hp_and_status(!data_is_for_ai, data.other.hp, data.other.status);
			}
		});
	}

	void try_correct_hp_and_status(bool const is_ai, bounded::optional<VisibleHP> const hp, bounded::optional<Statuses> const status, auto... maybe_index) {
		if (hp) {
			m_battle.correct_hp(is_ai, *hp, maybe_index...);
			if (hp->current == CurrentVisibleHP(0_bi)) {
				return;
			}
		}
		if (status) {
			m_battle.correct_status(is_ai, *status, maybe_index...);
		}
	}

	void validate_weather(NormalWeather const weather) const {
		if (weather != m_battle.weather()) {
			std::cerr << "Inconsistent weather. Received "sv << to_string(weather) << " but expected "sv << to_string(to_normal_weather(m_battle.weather())) << '\n';
		}
	}

	Moves determine_action() {
		if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
			throw std::runtime_error("Tried to determine an action with an empty team.");
		}

		m_analysis_logger << to_string(m_battle.ai()) << '\n';
		m_analysis_logger << "Seen " << to_string(m_battle.foe()) << '\n';
		auto predicted = predict_team(m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
		m_analysis_logger << "Predicted " << to_string(predicted) << '\n' << std::flush;

		return expectiminimax(
			Team<generation>(m_battle.ai()),
			predicted,
			m_battle.weather(),
			m_evaluate,
			Depth(m_depth, 1U),
			m_analysis_logger
		).name;
	}

	void send_move_impl(bool const is_switch, auto const switch_move, auto const move_index) {
		m_send_message(containers::concatenate<containers::string>(m_id, (is_switch ? containers::concatenate_view("|/switch "sv, to_string(switch_move())) : containers::concatenate_view("|/move "sv, to_string(move_index() + 1_bi)))));
	}

	void send_move(Moves const move) {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto switch_move = [&]{ return m_slot_memory[to_replacement(move)]; };
		auto move_index = [&]{ return get_move_index(m_battle.ai().pokemon().regular_moves(), move); };
		send_move_impl(is_switch(move), switch_move, move_index);
	}

	void send_random_move() {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto distribution = std::uniform_int_distribution(0, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team - 1_bi));
		auto const result = distribution(m_random_engine);

		auto switch_move = [=]{ return bounded::assume_in_range<TeamIndex>(result - max_moves_per_pokemon); };
		auto move_index = [=]{ return bounded::assume_in_range<containers::index_type<RegularMoves>>(result); };
		auto const is_switch = result >= max_moves_per_pokemon;
		send_move_impl(is_switch, switch_move, move_index);
	}

	void handle_u_turn(Party const party) {
		if (is_ai(party) and m_move_state.executed_move() == Moves::U_turn and m_battle.foe().pokemon().hp().current() == 0_bi and m_battle.ai().size() != 1_bi) {
			maybe_use_previous_move();
			send_move(determine_action());
		}
	}

	UsageStats const & m_usage_stats;
		
	SendMessageFunction m_send_message;
	BattleLogger m_battle_logger;
	std::ofstream m_analysis_logger;
	containers::string m_id;
	containers::string m_username;
	std::mt19937 m_random_engine;

	SlotMemory m_slot_memory;

	Evaluate<generation> m_evaluate;
	Battle<generation> m_battle;
	Party m_ai_party;
	DepthValues m_depth;
	MoveState m_move_state;
	EndOfTurnState m_end_of_turn_state;
	struct Switch {
		Moves move;
		VisibleHP hp;
		Statuses status;
	};
	bounded::optional<Switch> m_ai_switch;
	bounded::optional<Switch> m_foe_switch;
	bool m_log_foe_teams;
	bool m_completed = false;
	bool m_replacing_fainted = false;
};

} // namespace

auto make_battle_parser(
	SendMessageFunction send_message,
	BattleLogger battle_logger,
	std::ofstream analysis_logger,
	containers::string id,
	containers::string username,
	AllUsageStats const & usage_stats,
	AllEvaluate evaluate,
	Party party,
	DepthValues const depth,
	std::mt19937 random_engine,
	GenerationGeneric<Teams> const & generic_teams,
	bool log_foe_teams
) -> std::unique_ptr<BattleParser> {
	return bounded::visit(generic_teams, [&]<Generation generation>(Teams<generation> const & teams) -> std::unique_ptr<BattleParser> {
		return std::make_unique<BattleParserImpl<generation>>(
			std::move(send_message),
			std::move(battle_logger),
			std::move(analysis_logger),
			std::move(id),
			std::move(username),
			usage_stats[generation],
			evaluate.get<generation>(),
			party,
 			depth,
			random_engine,
			teams.ai,
			teams.foe,
			log_foe_teams
		);
	});
}


auto parse_details(std::string_view const details) -> ParsedDetails {
	auto parser = DelimitedBufferView(details, std::string_view(", "));
	auto const species = from_string<Species>(parser.pop());

	auto const level_or_gender_or_shiny_str = parser.pop();
	auto const has_level = !level_or_gender_or_shiny_str.empty() and level_or_gender_or_shiny_str.front() == 'L';
	auto const level = Level(has_level ?
		bounded::to_integer<Level::value_type>(level_or_gender_or_shiny_str.substr(1)) :
		100_bi
	);

	auto const gender_or_shiny_str = has_level ? parser.pop() : level_or_gender_or_shiny_str;
	auto const gender = 
		BOUNDED_CONDITIONAL(gender_or_shiny_str == "F", Gender::female,
		BOUNDED_CONDITIONAL(gender_or_shiny_str == "M", Gender::male,
		Gender::genderless
	));

	auto const shiny_str = gender != Gender::genderless ? parser.pop() : gender_or_shiny_str;
	auto throw_exception = [&] {
		throw std::runtime_error(containers::concatenate<std::string>("Invalid PS details string: "sv, details));
	};
	if (shiny_str != "shiny" and shiny_str != "") {
		throw_exception();
	}
	if (!parser.remainder().empty()) {
		throw_exception();
	}
	return ParsedDetails{species, level, gender};
}

auto parse_switch(InMessage message) -> ParsedSwitch {
	auto const identity = parse_identity(message.pop());
	auto const details = parse_details(message.pop());
	auto const [visible_hp, status] = parse_hp_and_status(message.pop());
	return ParsedSwitch{identity.party, details.species, identity.nickname, details.level, details.gender, visible_hp, status};
}

} // namespace ps
} // namespace technicalmachine
