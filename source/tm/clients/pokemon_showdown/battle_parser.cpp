// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/chat.hpp>

#include <tm/clients/log_foe_team.hpp>

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/move/moves.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/team.hpp>

#include <tm/visible_hp.hpp>

#include <bounded/integer.hpp>
#include <bounded/to_integer.hpp>
#include <bounded/detail/overload.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/algorithms/maybe_find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {

namespace {

constexpr auto party_from_player_id(std::string_view const player_id) {
	return make_party(player_id.substr(0, 2));
}

auto get_move_index(Pokemon const & pokemon, Moves const move_name) {
	auto const moves = pokemon.all_moves().regular();
	auto const it = containers::find_if(moves, [=](Move const move) { return move.name() == move_name; });
	if (it == end(moves)) {
		throw std::runtime_error("Pokemon does not know " + std::string(to_string(move_name)));
	}
	return static_cast<RegularMoveIndex>(it - begin(moves));
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

using EffectSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous, FromRecoil>;

constexpr auto parse_effect_source(std::string_view const type, std::string_view const source) {
	return
		(type == "") ? EffectSource(MainEffect{}) :
		(type == "item") ? EffectSource(from_string<Item>(source)) :
		(type == "ability") ? EffectSource(from_string<Ability>(source)) :
		(type == "move") ? EffectSource(FromMove{}) :
		(type == "confusion") ? EffectSource(FromConfusion{}) :
		(type == "Recoil") ? EffectSource(FromRecoil{}) :
		(
			type == "brn" or
			type == "psn" or
			type == "tox" or
			type == "drain" or
			type == "Encore" or
			type == "Leech Seed" or
			type == "Light Screen" or
			type == "Spikes" or
			type == "Stealth Rock" or
			type == "Hail" or
			type == "Reflect" or
			type == "Sandstorm" or
			type == "Substitute"
		) ? EffectSource(FromMiscellaneous{}) :
		throw std::runtime_error("Unhandled effect source type: " + std::string(type));
}

auto parse_from_source(InMessage message) {
	// "[from]" or nothing
	auto const from [[maybe_unused]] = message.pop(' ');
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

} // namespace


void BattleParser::handle_message(InMessage message) {
	m_battle_logger.log(message);
	
	if (handle_chat_message(message)) {
		return;
	}

	auto const type = message.type();
	if (type.empty() or (type.front() != '-' and type != "drag" and type != "move")) {
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
		m_battle.set_value_on_active(is_ai(party), ability);
	} else if (type == "-activate") {
		auto const party = party_from_player_id(message.pop());
		auto const what = message.pop();
		auto const details = message.pop();
		if (what == "Substitute") {
			if (details == "[damage]") {
				m_move_state.damage_substitute(other(party));
				handle_u_turn(other(party));
			}
		} else {
			// what = ability: Forewarn
			// details = Earthquake
			// remainder() = [of] p1a: Excadrill
		}
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
	} else if (type == "callback") {
		// Looks like it's just another form of "cant", but it is probably used
		// for more things than just that
	} else if (type == "cant") {
		// TODO: Figure out how to solve this in general...
		auto const party = party_from_player_id(message.pop());
		auto const & team = get_team(party);
		auto const reason = message.pop();
		auto const move = reason == "recharge" ? team.pokemon().last_used_move().name() : Moves::Struggle;
		m_move_state.use_move(party, move);
	} else if (type == "-center") {
	} else if (type == "-clearallboost") {
		// We already know what Haze does
	} else if (type == "-crit") {
		auto const user = other(party_from_player_id(message.pop()));
		m_move_state.critical_hit(user);
	} else if (type == "-curestatus") {
		auto const party = party_from_player_id(message.pop());
		if (m_move_state.party() != party) {
			maybe_use_previous_move();
			m_move_state.clear_status(party);
			if (m_battle.generation() == Generation::one) {
				// TODO: Try to do something smarter here
				m_move_state.use_move(party, Moves::Struggle);
			}
			auto const source = parse_from_source(message);
			bounded::visit(source, bounded::overload(
				[&](Ability const ability) { m_battle.set_value_on_active(is_ai(party), ability); },
				[](auto) { }
			));
		}
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
		m_battle.set_value_on_active(is_ai(party), item);
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
		m_battle.handle_fainted(is_ai(party));
		if (is_ai(party) and m_battle.ai().size() != 1_bi) {
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
			[&](auto const value) { m_battle.set_value_on_active(is_ai(party), value); }
		));
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
			[&](auto const value) { m_battle.set_value_on_active(is_ai(party), value); }
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
		m_battle.set_value_on_active(is_ai(party), item);
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
	} else if (type == "switch" or type == "drag") {
		auto const parsed = parse_switch(message);

		if (is_ai(parsed.party)) {
			auto const index = find_present_index(m_battle.ai().all_pokemon(), parsed.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		auto const move = m_battle.find_or_add_pokemon(is_ai(parsed.party), parsed.species, parsed.level, parsed.gender);
		m_battle.correct_hp_and_status(
			is_ai(parsed.party),
			parsed.hp,
			parsed.status,
			to_replacement(move)
		);
		if (type == "drag") {
			m_move_state.phaze_index(other(parsed.party), get_team(parsed.party), parsed.species);
		} else {
			m_move_state.use_move(parsed.party, move);
		}
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
					// TODO
				} else {
					m_move_state.confuse();
				}
			},
			[](FromMiscellaneous) {},
			[](FromMove) {},
			[](FromRecoil) { throw std::runtime_error("Unexpected -start source FromRecoil"); },
			[&](auto const value) { m_battle.set_value_on_active(is_ai(party), value); }
		));
	} else if (type == "-status") {
		auto const party = party_from_player_id(message.pop());
		auto const status = parse_status(message.pop());
		auto const source = parse_from_source(message);
		bounded::visit(source, bounded::overload(
			[&](MainEffect) { m_move_state.status(party, status); },
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
			[&](FromMiscellaneous) { m_move_state.status(other(party), status); },
			[&](FromMove) { m_move_state.status(party, status); },
			[](FromRecoil) { throw std::runtime_error("Recoil cannot cause another status"); },
			[&](auto const value) { m_battle.set_value_on_active(is_ai(party), value); }
		));
	} else if (type == "swap") {
#if 0
		auto const pokemon = message.pop();
		auto const position = message.pop();
		// position here is "0", "1", or "2"
#endif
	} else if (type == "tie") {
		m_completed = true;
	} else if (type == "-transform") {
		// message.remainder() == POKEMON|SPECIES
	} else if (type == "turn") {
		auto const turn = bounded::to_integer<0, bounded::max_value<std::uint32_t>>(message.pop());
		m_analysis_logger << std::string(20, '=') << "\nBegin turn " << turn << '\n';
		m_battle.handle_begin_turn();
		send_move(determine_action());
	} else if (type == "-unboost") {
#if 0
		auto const pokemon = message.pop();
		auto const stat = message.pop();
		auto const amount = message.pop();
#endif
	} else if (type == "upkeep") {
		m_battle.handle_end_turn();
	} else if (type == "-weather") {
#if 0
		auto const weather = message.pop();
#endif
	} else if (type == "win") {
		if (m_log_foe_teams) {
			auto const won = m_username == message.pop();
			if (!won) {
				log_foe_team(m_battle.generation(), m_usage_stats, m_battle.foe(), m_random_engine);
			}
		}
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
	}
}

void BattleParser::handle_damage(InMessage message) {
	auto const parsed = parse_hp_message(message);
	auto const hp_and_status = HPAndStatus{parsed.hp, parsed.status};
	auto move_damage = [&](Party const party) {
		m_move_state.damage(party, hp_and_status);
	};
	auto const party = parsed.party;
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
		[&](auto const value) { m_battle.set_value_on_active(is_ai(party), value); }
	));
	m_move_state.hp_change(party, hp_and_status);
}

namespace {

auto hp_to_damage(Pokemon const & pokemon, HP::current_type const new_hp) {
	auto const old_hp = pokemon.hp().current();
	if (new_hp > old_hp) {
		std::cerr << "Took negative damage\n";
		return HP::current_type(0_bi);
		// throw std::runtime_error("Took negative damage");
	}
	return HP::current_type(old_hp - new_hp);
}

auto const & select_pokemon(Team const & team, Moves const move) {
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

} // namespace

auto BattleParser::compute_damage(Party const user_party, Moves const move, HPAndStatus const damage) const -> HP::current_type {
	auto const damaged_party = move == Moves::Hit_Self ? user_party : other(user_party);
	auto const & pokemon = select_pokemon(get_team(damaged_party), move);
	auto const new_hp = to_real_hp(is_ai(damaged_party), pokemon.hp(), damage.hp);
	return hp_to_damage(pokemon, new_hp.value);
}

void BattleParser::maybe_use_previous_move() {
	auto const maybe_data = m_move_state.complete();
	if (!maybe_data) {
		return;
	}
	auto const data = *maybe_data;
	
	if (causes_recoil(data.move.executed) and !data.recoil) {
		// TODO: This could also be Magic Guard
		m_battle.set_value_on_active(is_ai(data.party), Ability::Rock_Head);
	}
	
	auto const other_pokemon = get_team(other(data.party)).pokemon();

	struct LocalDamage {
		ActualDamage value;
		bool did_any_damage;
	};
	auto const damage = bounded::visit(data.damage, bounded::overload(
		[](MoveState::NoDamage) {
			return LocalDamage{ActualDamage::Known{0_bi}, false};
		},
		[&](HPAndStatus const hp_and_status) -> LocalDamage {
			auto const value = compute_damage(data.party, data.move.executed, hp_and_status);
			return LocalDamage{
				ActualDamage::Known{value},
				value != 0_bi
			};
		},
		[&](MoveState::SubstituteDamaged) -> LocalDamage {
			return LocalDamage{ActualDamage::Capped{other_pokemon.hp().max() / 4_bi}, true};
		},
		[&](MoveState::SubstituteBroke) -> LocalDamage {
			return LocalDamage{ActualDamage::Known{other_pokemon.substitute().hp()}, true};
		}
	));
	
	auto const other_move = other_pokemon.moved() ?
		OtherMove([&]{
			auto const move = other_pokemon.last_used_move().name();
			auto const type = get_type(m_battle.generation(), move, get_hidden_power_type(other_pokemon));
			return KnownMove{move, type};
		}()) :
		OtherMove(FutureMove{
			data.move.executed == Moves::Sucker_Punch and damage.did_any_damage
		});

	m_battle.handle_use_move(is_ai(data.party), data.move, data.clear_status, damage.value, other_move);

	if (data.user_hp_and_status) {
		m_battle.correct_hp_and_status(
			is_ai(data.party),
			data.user_hp_and_status->hp,
			data.user_hp_and_status->status
		);
	}
	if (data.other_hp_and_status) {
		m_battle.correct_hp_and_status(
			!is_ai(data.party),
			data.other_hp_and_status->hp,
			data.other_hp_and_status->status
		);
	}
}

Moves BattleParser::determine_action() {
	if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}

	m_analysis_logger << to_string(m_battle.generation(), m_battle.ai()) << '\n';
	m_analysis_logger << "Seen " << to_string(m_battle.generation(), m_battle.foe()) << '\n';
	auto predicted = predict_team(m_battle.generation(), m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
	m_analysis_logger << "Predicted " << to_string(m_battle.generation(), predicted) << '\n' << std::flush;

	return expectiminimax(m_battle.generation(), m_battle.ai(), predicted, m_battle.weather(), m_evaluate, Depth(m_depth, 1U), m_analysis_logger).name;
}

void BattleParser::send_move(Moves const move) {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	using std::to_string;
	auto switch_move = [&]{ return to_string(m_slot_memory[to_replacement(move)]); };
	auto move_index = [&]{ return to_string(get_move_index(m_battle.ai().all_pokemon()(), move) + 1_bi); };
	m_send_message(m_id + (is_switch(move) ? "|/switch " + switch_move() : "|/move " + move_index()));
}

void BattleParser::send_random_move() {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	auto distribution = std::uniform_int_distribution(1, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team));
	auto const result = distribution(m_random_engine);

	auto switch_move = [=]{ return std::to_string(result - max_moves_per_pokemon); };
	auto move_index = [=]{ return std::to_string(result); };
	auto const is_switch = result > max_moves_per_pokemon;
	m_send_message(m_id + (is_switch ? "|/switch " + switch_move() : "|/move " + move_index()));
}

auto parse_switch(InMessage message) -> ParsedSwitch {
	auto const party = party_from_player_id(message.pop());
	auto const details = parse_details(message.pop());
	auto const [visible_hp, status] = parse_hp_and_status(message.pop());
	return ParsedSwitch{party, details.species, details.level, details.gender, visible_hp, status};
}

void BattleParser::handle_u_turn(Party const party) {
	if (is_ai(party) and m_move_state.executed_move() == Moves::U_turn and m_battle.foe().pokemon().hp().current() == 0_bi and m_battle.ai().size() != 1_bi) {
		maybe_use_previous_move();
		send_move(determine_action());
	}
}

}	// namespace ps
}	// namespace technicalmachine
