// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>
#include <tm/clients/pokemon_showdown/end_of_turn_state.hpp>
#include <tm/clients/pokemon_showdown/move_state.hpp>
#include <tm/clients/pokemon_showdown/slot_memory.hpp>

#include <tm/clients/battle_manager.hpp>
#include <tm/clients/log_foe_team.hpp>

#include <tm/move/move_name.hpp>

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move_name.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status_name.hpp>
#include <tm/string_conversions/team.hpp>
#include <tm/string_conversions/weather.hpp>

#include <tm/battle.hpp>
#include <tm/generation_generic.hpp>
#include <tm/status_name.hpp>
#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>
#include <bounded/overload.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/algorithms/concatenate_view.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/begin_end.hpp>
#include <containers/index_type.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <iostream>
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

constexpr auto parse_status(std::string_view const str) {
	return
		(str == "") ? StatusName::clear :
		(str == "fnt") ? StatusName::clear :
		(str == "brn") ? StatusName::burn :
		(str == "frz") ? StatusName::freeze :
		(str == "par") ? StatusName::paralysis :
		(str == "psn") ? StatusName::poison :
		(str == "tox") ? StatusName::toxic :
		(str == "slp") ? StatusName::sleep :
//		(str == "") ? StatusName::rest :
		throw std::runtime_error("Received an invalid status");
}

struct HPAndStatus {
	VisibleHP hp;
	StatusName status;
};

constexpr auto parse_hp_and_status(std::string_view const hp_and_status) {
	auto const [hp_fraction, status] = split_view(hp_and_status, ' ');
	return HPAndStatus{parse_hp(hp_fraction), parse_status(status)};
}

struct FromMove {};
struct MainEffect {};
struct FromConfusion {};
struct FromMiscellaneous {};
struct FromRecoil {};
struct FromSubstitute {};

using EffectSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous, FromRecoil, FromSubstitute>;

constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) -> EffectSource {
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

constexpr auto parse_from_source(std::string_view const message) -> EffectSource {
	// [from]
	auto const [bracketed_text_or_nothing, remainder] = split_view(message, ' ');
	auto const [type, source] = split_view(remainder, ':');
	return parse_effect_source(type, source);
}

constexpr auto parse_hp_message(InMessage message) {
	struct Message {
		Party party;
		VisibleHP hp;
		StatusName status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message.pop());
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

constexpr auto parse_set_hp_message(InMessage message) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct Message {
		Party party;
		VisibleHP hp;
		StatusName status;
		EffectSource source;
	};
	auto const party = party_from_player_id(message.pop());
	auto const hp_and_status = parse_hp_and_status(message.pop());
	auto const source = parse_from_source(message.pop());

	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

struct BattleParserImpl : BattleParser {
	BattleParserImpl(
		SendMessageFunction send_message,
		BattleLogger battle_logger,
		std::ofstream analysis_logger,
		containers::string id_,
		containers::string username,
		AllUsageStats const & usage_stats,
		AllEvaluate evaluate,
		Party party,
		DepthValues const depth,
		std::mt19937 random_engine,
		GenerationGeneric<Teams> generic_teams,
		bool log_foe_teams
	):
		m_slot_memory(bounded::visit(generic_teams, [](auto const & teams) { return teams.ai.size(); })),
		m_battle_manager(make_battle_manager(
			std::move(analysis_logger),
			usage_stats,
			std::move(evaluate),
			depth,
			std::move(random_engine),
			std::move(generic_teams),
			log_foe_teams
		)),
		m_send_message(std::move(send_message)),
		m_battle_logger(std::move(battle_logger)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_ai_party(party)
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
			auto const source = parse_from_source(message.pop());
			maybe_commit_switch(party);
			bounded::visit(source, bounded::overload(
				[&](MainEffect) { set_value_on_pokemon(party, ability); },
				[&](Ability const source_ability) {
					switch (source_ability) {
						case Ability::Trace:
							set_value_on_pokemon(party, source_ability);
							set_value_on_pokemon(other(party), ability);
							break;
						default:
							throw std::runtime_error("Invalid ability to get another ability");
					}
				},
				[](auto) { throw std::runtime_error("Invalid ability source"); }
			));
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
						handle_delayed_switch(other(party));
					}
				},
				[&](Ability const ability) {
					set_value_on_pokemon(party, ability);
					switch (ability) {
						case Ability::Forewarn:
							m_battle_manager->active_has(
								!is_ai(party),
								from_string<MoveName>(details)
							);
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
			if (reason == "flinch") {
				m_move_state.flinch(party);
			} else if (reason == "frz") {
				m_move_state.frozen_solid(party);
			} else if (reason == "par") {
				m_move_state.fully_paralyze(party);
			} else if (reason == "slp") {
				m_move_state.still_asleep(party);
			} else if (reason == "recharge") {
				m_move_state.recharge(party);
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
			if (m_ignore_next_cure_status) {
				m_ignore_next_cure_status = false;
				return;
			}
			auto const party = party_from_player_id(message.pop());
			auto const status = parse_status(message.pop());
			auto const source = parse_from_source(message.pop());
			bounded::visit(source, bounded::overload(
				[&](MainEffect) {
					if (m_battle_manager->is_end_of_turn()) {
						m_end_of_turn_state.set_expected(party, StatusName::clear);
						return;
					}
					auto const move_name = m_move_state.executed_move();
					auto const move_cured_status = move_name and (
						m_move_state.party() == party or
						m_battle_manager->cures_target_status(is_ai(party), *move_name)
					);
					if (move_cured_status) {
						m_move_state.status_from_move(party, StatusName::clear);
					} else {
						maybe_use_previous_move();
						switch (status) {
							case StatusName::freeze:
								m_move_state.thaw(party);
								break;
							case StatusName::sleep:
								m_move_state.awaken(party, m_battle_manager->generation());
								break;
							default:
								throw std::runtime_error("Spontaneously recovered from status");
						}

					}
				},
				[&](Ability const ability) {
					set_value_on_pokemon(party, ability);
					switch (ability) {
						case Ability::Natural_Cure:
							break;
						case Ability::Shed_Skin:
							m_end_of_turn_state.set_expected(party, StatusName::clear);
							break;
						default:
							throw std::runtime_error("Unexpected ability cured status");
					}
				},
				[](auto) { throw std::runtime_error("Unexpected -curestatus source"); }
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
				handle_delayed_switch(other(party));
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
			for (auto const status : containers::enum_range<StatusName>()) {
				if (clears_status(item, status)) {
					m_ignore_next_cure_status = true;
					queue_hp_or_status_checks(party, StatusName::clear);
					break;
				}
			}
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
			if (m_battle_manager->generation() <= Generation::three and is_ai(party) and !m_battle_manager->ai_is_on_last_pokemon() and !m_battle_manager->is_end_of_turn()) {
				m_replacing_fainted = true;
				send_move(m_battle_manager->determine_action());
			}
		} else if (type == "-fieldactivate") {
			// move: Perish Song
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
			auto const source = parse_from_source(message.pop());
			bounded::visit(source, bounded::overload(
				// TODO: Validate that the type should be immune
				[](MainEffect) {},
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause immunity"); },
				[](FromMiscellaneous) { throw std::runtime_error("Miscellaneous effects cannot cause immunity"); },
				[](FromMove) { throw std::runtime_error("MoveName cannot cause immunity"); },
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
			auto const move = from_string<MoveName>(message.pop());
#if 0
			// target is sent only for moves that target one Pokemon
			auto const target = message.pop();
#endif
			if (m_move_state.party() != party) {
				maybe_use_previous_move();
			}
			m_move_state.use_move(party, move);
			if (is_ai(party) and move == MoveName::Baton_Pass and !m_battle_manager->ai_is_on_last_pokemon()) {
				maybe_use_previous_move();
				send_move(m_battle_manager->determine_action());
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
		} else if (type == "-setboost") {
			// Belly Drum
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
			m_move_state.phazed_in(other(parsed.party), to_replacement(parsed.move));
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
			auto parse_remainder_of_source = [&] {
				if (first_part_of_source == "typechange") {
					auto const changed_type = message.pop();
					static_cast<void>(changed_type);
					return parse_from_source(message.pop());
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
						m_move_state.confuse(other(party));
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
			auto const source = parse_from_source(message.pop());
			maybe_commit_switch(party);
			bounded::visit(source, bounded::overload(
				[&](MainEffect) { m_move_state.status_from_move(party, status); },
				[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
				[&](FromMiscellaneous) { m_move_state.status_from_move(other(party), status); },
				[&](FromMove) { m_move_state.status_from_move(party, status); },
				[](FromRecoil) { throw std::runtime_error("Recoil cannot cause another status"); },
				[](FromSubstitute) { throw std::runtime_error("Substitute cannot cause another status"); },
				[&](Ability const ability) {
					set_value_on_pokemon(other(party), ability);
					m_move_state.status_from_contact_ability(party, ability, status);
				},
				[&](Item const item) {
					set_value_on_pokemon(party, item);
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
			m_battle_manager->complete(Result::tied);
		} else if (type == "-transform") {
			// message.remainder() == POKEMON|SPECIES
		} else if (type == "turn") {
			auto const turn_count = bounded::to_integer<TurnCount>(message.pop());
			m_battle_manager->begin_turn(turn_count);
			send_move(m_battle_manager->determine_action());
		} else if (type == "-unboost") {
#if 0
			auto const pokemon = message.pop();
			auto const stat = message.pop();
			auto const amount = message.pop();
#endif
		} else if (type == "upkeep") {
			auto const end_of_turn_state = m_end_of_turn_state.complete();
			bool const ai_went_first = end_of_turn_state.first_party == m_ai_party;
			m_battle_manager->end_turn(ai_went_first, end_of_turn_state.first.flags, end_of_turn_state.last.flags);
			m_battle_manager->weather_is(end_of_turn_state.weather);
			try_correct_hp_and_status(ai_went_first, end_of_turn_state.first.hp, end_of_turn_state.first.status);
			try_correct_hp_and_status(!ai_went_first, end_of_turn_state.last.hp, end_of_turn_state.last.status);
			if (m_battle_manager->ai_is_fainted()) {
				m_replacing_fainted = true;
				send_move(m_battle_manager->determine_action());
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
			auto const won = m_username == message.pop();
			m_battle_manager->complete(won ? Result::won : Result::lost);
		} else {
			std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
		}
	}

	std::string_view id() const final {
		return m_id;
	}
	bool completed() const final {
		return m_battle_manager->completed();
	}

private:
	auto is_ai(Party const party) const -> bool {
		return party == m_ai_party;
	}

	auto handle_damage(InMessage message) -> void {
		auto const parsed = parse_hp_message(message);
		auto move_damage = [&](Party const party) {
			m_move_state.damage(party, parsed.hp);
		};
		auto const party = parsed.party;
		maybe_commit_switch(party);
		bounded::visit(parsed.source, bounded::overload(
			[&](MainEffect) {
				if (m_move_state.move_damaged_self(party)) {
					return;
				}
				move_damage(other(party));
				handle_delayed_switch(other(party));
			},
			[&](FromConfusion) {
				// TODO: Technically you cannot select Hit Self, you just execute
				// it. This matters for things like priority or determining whether
				// Sucker Punch succeeds. As a workaround for now, say the user
				// selected Struggle.
				// When hitting self in confusion, we get -activate then -damage
				maybe_use_previous_move();
				m_move_state.use_move(party, MoveName::Struggle);
				m_move_state.use_move(party, MoveName::Hit_Self);
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

	struct SwitchOrDragResult {
		Party party;
		Species species;
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto handle_switch_or_drag(InMessage message) -> SwitchOrDragResult {
		auto parsed = parse_switch(message);
		auto const data_is_for_ai = is_ai(parsed.party);
		auto const move = [&] {
			if (data_is_for_ai) {
				auto const result = m_battle_manager->ai_has(
					parsed.species,
					std::move(parsed).nickname,
					parsed.level,
					parsed.gender
				);
				auto const switch_index = to_replacement(result);
				if (m_replacing_fainted) {
					m_slot_memory.replace_fainted(switch_index);
					m_replacing_fainted = false;
				} else {
					m_slot_memory.switch_to(switch_index);
				}
				return result;
			} else {
				return m_battle_manager->foe_has(
					parsed.species,
					std::move(parsed).nickname,
					parsed.level,
					parsed.gender
				);
			}
		}();

		return SwitchOrDragResult{parsed.party, parsed.species, move, parsed.hp, parsed.status};
	}

	void set_value_on_pokemon(Party const party, auto const value) {
		auto const for_ai = is_ai(party);
		if (auto const replacing = m_move_state.switch_index()) {
			m_battle_manager->replacement_has(for_ai, *replacing, value);
		} else {
			m_battle_manager->active_has(for_ai, value);
		}
	}

	void queue_hp_or_status_checks(Party const party, auto const value) {
		if (m_move_state.party()) {
			m_move_state.set_expected(party, value);
		} else {
			m_end_of_turn_state.set_expected(party, value);
		}
	}

	struct Switch {
		MoveName move;
		VisibleHP hp;
		StatusName status;
	};
	auto get_switch(Party const party) -> bounded::optional<Switch> & {
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
		auto const maybe_data = m_move_state.complete();
		if (!maybe_data) {
			return;
		}
		auto & data = *maybe_data;
		auto const data_is_for_ai = data.party == m_ai_party;
		m_battle_manager->use_move(data_is_for_ai, data.move, data.user_status_was_cleared); 
		try_correct_hp_and_status(data_is_for_ai, data.user.hp, data.user.status);
		try_correct_hp_and_status(!data_is_for_ai, data.other.hp, data.other.status);
	}

	void try_correct_hp_and_status(bool const is_ai, bounded::optional<VisibleHP> const hp, bounded::optional<StatusName> const status, auto... maybe_index) {
		if (hp) {
			m_battle_manager->correct_hp(is_ai, *hp, maybe_index...);
			if (hp->current == CurrentVisibleHP(0_bi)) {
				return;
			}
		}
		if (status) {
			m_battle_manager->correct_status(is_ai, *status, maybe_index...);
		}
	}

	void send_move_impl(bool const is_switch, auto const switch_move, auto const move_index) {
		m_send_message(containers::concatenate<containers::string>(m_id, (is_switch ? containers::concatenate_view("|/switch "sv, to_string(switch_move())) : containers::concatenate_view("|/move "sv, to_string(move_index() + 1_bi)))));
	}

	void send_move(MoveName const move) {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto switch_move = [&]{ return m_slot_memory[to_replacement(move)]; };
		auto move_index = [&]{ return m_battle_manager->move_index(move); };
		send_move_impl(is_switch(move), switch_move, move_index);
	}

	void send_random_move() {
		// In doubles / triples we need to specify " TARGET" at the end for regular
		// moves
		auto distribution = std::uniform_int_distribution(0, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team - 1_bi));
		auto const result = distribution(m_battle_manager->random_engine());

		auto switch_move = [=]{ return bounded::assume_in_range<TeamIndex>(result - max_moves_per_pokemon); };
		auto move_index = [=]{ return bounded::assume_in_range<containers::index_type<RegularMoves>>(result); };
		auto const is_switch = result >= max_moves_per_pokemon;
		send_move_impl(is_switch, switch_move, move_index);
	}

	void handle_delayed_switch(Party const party) {
		if (!is_ai(party)) {
			return;
		}
		auto const executed_move = m_move_state.executed_move();
		if (!executed_move) {
			return;
		}
		// TODO: What is the correct check here
		if (is_delayed_switch(*executed_move) and !m_battle_manager->foe_is_fainted() and !m_battle_manager->ai_is_on_last_pokemon()) {
			maybe_use_previous_move();
			send_move(m_battle_manager->determine_action());
		}
	}

	SlotMemory m_slot_memory;
	std::unique_ptr<BattleManager> m_battle_manager;

	SendMessageFunction m_send_message;
	BattleLogger m_battle_logger;
	containers::string m_id;
	containers::string m_username;

	Party m_ai_party;
	MoveState m_move_state;
	EndOfTurnState m_end_of_turn_state;

	bounded::optional<Switch> m_ai_switch;
	bounded::optional<Switch> m_foe_switch;

	bool m_ignore_next_cure_status = false;
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
	AllEvaluate const evaluate,
	Party const party,
	DepthValues const depth,
	std::mt19937 random_engine,
	GenerationGeneric<Teams> teams,
	bool const log_foe_teams
) -> std::unique_ptr<BattleParser> {
	return std::make_unique<BattleParserImpl>(
		std::move(send_message),
		std::move(battle_logger),
		std::move(analysis_logger),
		std::move(id),
		std::move(username),
		usage_stats,
		evaluate,
		party,
		depth,
		std::move(random_engine),
		std::move(teams),
		log_foe_teams
	);
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
