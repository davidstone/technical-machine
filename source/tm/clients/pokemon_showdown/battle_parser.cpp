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

#include <tm/move/damage_type.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/integer.hpp>
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
#include <variant>

namespace technicalmachine {
namespace ps {

namespace {

constexpr auto party_from_player_id(std::string_view const player_id) {
	return make_party(player_id.substr(0, 2));
}

template<typename Container, typename Value>
constexpr auto container_index(Container const & container, Value const value, std::string_view const text) {
	auto const it = containers::find(container, value);
	if (it == end(container)) {
		throw std::runtime_error(std::string(text) + std::string(to_string(value)));
	}
	return static_cast<containers::index_type<Container>>(it - begin(container));
}

auto get_move_index(Pokemon const & pokemon, Moves const move) {
	return container_index(all_moves(pokemon).regular(), move, "Pokemon does not know ");
}
auto get_species_index(Team const & team, Species const species) {
	return container_index(team.all_pokemon(), species, "Team does not have a ");
}

auto const & get_team(Battle const & battle, Party const party) {
	return battle.is_me(party) ? battle.ai() : battle.foe();
}

constexpr auto parse_status(std::string_view const str) {
	return
		(str == "") ? Statuses::clear :
		(str == "fnt") ? Statuses::clear :
		(str == "brn") ? Statuses::burn :
		(str == "frz") ? Statuses::freeze :
		(str == "par") ? Statuses::paralysis :
		(str == "psn") ? Statuses::poison :
		(str == "tox") ? Statuses::poison_toxic :
		(str == "slp") ? Statuses::sleep :
//		(str == "") ? Statuses::sleep_rest :
		throw std::runtime_error("Received an invalid status");
}

auto parse_hp_and_status(std::string_view const hp_and_status) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	// TODO: validate that the status we received matches our expected status
	struct Result {
		HP::current_type current_hp;
		HP::max_type max_hp;
		Statuses status;
	};
	auto const [hp_fraction, status] = split(hp_and_status, ' ');
	auto const [remaining_visible_hp, max_visible_hp] = split(hp_fraction, '/');
	return Result{
		bounded::to_integer<HP::current_type>(remaining_visible_hp),
		max_visible_hp.empty() ? 100_bi : bounded::to_integer<HP::max_type>(max_visible_hp),
		parse_status(status)
	};
}

struct FromMove{};
struct MainEffect{};
struct FromConfusion{};
struct FromMiscellaneous{};

using HPChangeSource = bounded::variant<MainEffect, Item, Ability, FromMove, FromConfusion, FromMiscellaneous>;
auto parse_hp_change_source(InMessage message) {
	using Source = HPChangeSource;
	// "[from]" or nothing
	auto const from [[maybe_unused]] = message.next(' ');
	auto const [source_type, source] = split(message.next(), ':');
	return
		(source_type == "") ? Source(MainEffect{}) :
		(source_type == "item") ? Source(from_string<Item>(source)) :
		(source_type == "ability") ? Source(from_string<Ability>(source)) :
		(source_type == "move") ? Source(FromMove{}) :
		(source_type == "confusion") ? Source(FromConfusion{}) :
		(
			source_type == "brn" or
			source_type == "psn" or
			source_type == "tox" or
			source_type == "drain" or
			source_type == "Leech Seed" or
			source_type == "Recoil" or
			source_type == "Spikes" or
			source_type == "Stealth Rock" or
			source_type == "Hail" or
			source_type == "Sandstorm"
		) ? Source(FromMiscellaneous{}) :
		throw std::runtime_error("Unhandled HP change source type: " + std::string(source_type));
}

auto parse_hp_message(InMessage message) {
	struct Message {
		Party party;
		HP::current_type current_hp;
		HP::max_type max_hp;
		Statuses status;
		HPChangeSource source;
	};
	auto const party = party_from_player_id(message.next());
	auto const hp_and_status = parse_hp_and_status(message.next());
	auto const source = parse_hp_change_source(message);
	return Message{
		party,
		hp_and_status.current_hp,
		hp_and_status.max_hp,
		hp_and_status.status,
		source
	};
}

auto parse_set_hp_message(InMessage message) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct MessagePokemon {
		Party party;
		HP::current_type current_hp;
		HP::max_type max_hp;
		Statuses status;
	};
	struct Message {
		MessagePokemon pokemon1;
		MessagePokemon pokemon2;
		HPChangeSource source;
	};
	auto const party1 = party_from_player_id(message.next());
	auto const hp_and_status1 = parse_hp_and_status(message.next());
	auto const party2 = party_from_player_id(message.next());
	auto const hp_and_status2 = parse_hp_and_status(message.next());
	auto const source = parse_hp_change_source(message);
	
	return Message{
		{party1, hp_and_status1.current_hp, hp_and_status1.max_hp, hp_and_status1.status},
		{party2, hp_and_status2.current_hp, hp_and_status1.max_hp, hp_and_status2.status},
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
		auto const party = party_from_player_id(message.next());
		auto const ability = from_string<Ability>(message.next());
		m_battle.set_value_on_active(party, ability);
	} else if (type == "-activate") {
	} else if (type == "-boost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (type == "callback") {
		// Looks like it's just another form of "cant", but it is probably used
		// for more things than just that
	} else if (type == "cant") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == "REASON" or "REASON|MOVE"
#endif
	} else if (type == "-center") {
	} else if (type == "-crit") {
		auto const user = other(party_from_player_id(message.next()));
		m_move_state.critical_hit(user);
	} else if (type == "-curestatus") {
		auto const party = party_from_player_id(message.next());
		if (m_move_state.party() != party) {
			maybe_use_previous_move();
			m_move_state.clear_status(party);
		}
	} else if (type == "-cureteam") {
#if 0
		auto const pokemon = message.next();
#endif
	} else if (type == "-damage") {
		handle_damage(message);
	} else if (type == "detailschange" or type == "-formechange") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (type == "-end") {
		// Tells me if the substitute is destroyed, probably other stuff ending
		// message.remainder() == PLAYER: POKEMON|EFFECT
	} else if (type == "-endability") {
#if 0
		auto const pokemon = message.next();
		auto const ability = message.next();
#endif
	} else if (type == "-enditem") {
#if 0
		auto const pokemon = message.next();
		auto const item = message.next();
#endif
	} else if (type == "error") {
		if (message.remainder() != "[Invalid choice] There's nothing to choose") {
			send_random_move();
		}
	} else if (type == "-fail") {
#if 0
		auto const party = party_from_player_id(message.next());
		// There can be more to this message, but nothing we care about
#endif
	} else if (type == "faint") {
		constexpr auto slot = 0;
		auto const party = party_from_player_id(message.next());
		m_battle.handle_fainted(party, slot);
		if (m_battle.is_me(party) and (m_battle.ai().size() != 1_bi or get_hp(m_battle.ai().pokemon()) != 0_bi)) {
			m_replacing_fainted = true;
			send_move(determine_action());
		}
	} else if (type == "-fieldend") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
#endif
	} else if (type == "-fieldstart") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
#endif
	} else if (type == "-heal") {
		auto const parsed = parse_hp_message(message);
		auto const party = parsed.party;
		bounded::visit(parsed.source, bounded::overload(
			[](MainEffect) {},
			[](FromConfusion) { throw std::runtime_error("Confusion cannot heal"); },
			[](FromMiscellaneous) {},
			[](FromMove) {},
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "-hint") {
		// message.remainder() == MESSAGE
	} else if (type == "-hitcount") {
		auto const party = party_from_player_id(message.next());
		auto const count = bounded::to_integer<2, 5>(message.next());
		static_cast<void>(party);
		static_cast<void>(count);
		// TODO: Implement multi-hit moves
	} else if (type == "-immune") {
		auto const party = party_from_player_id(message.next());
		auto const source = parse_hp_change_source(message);
		bounded::visit(source, bounded::overload(
			[](MainEffect) {},
			[&](Ability const ability) { m_battle.set_value_on_active(party, ability); },
			[](auto const &) { throw std::runtime_error("Surprising source of immunity."); }
		));
	} else if (type == "inactive") {
		// message.remainder() == MESSAGE
		// Timer is on
	} else if (type == "inactiveoff") {
		// message.remainder() == MESSAGE
		// Timer is off
	} else if (type == "-item") {
		auto const party = party_from_player_id(message.next());
		auto const item = from_string<Item>(message.next());
		m_battle.set_value_on_active(party, item);
	} else if (type == "-mega") {
#if 0
		auto const pokemon = message.next();
		auto const megastone = message.next();
#endif
	} else if (type == "-message") {
	} else if (type == "-miss") {
		auto const user_party = party_from_player_id(message.next());
		m_move_state.miss(user_party);
	} else if (type == "move") {
		auto const party = party_from_player_id(message.next());
		auto const move = from_string<Moves>(message.next());
#if 0
		// target is sent only for moves that target one Pokemon
		auto const target = message.next();
#endif
		if (m_move_state.party() != party) {
			maybe_use_previous_move();
		}
		m_move_state.use_move(party, move);
		if (m_battle.is_me(party) and move == Moves::Baton_Pass) {
			maybe_use_previous_move();
			send_move(determine_action());
		}
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
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (type == "-sidestart") {
#if 0
		// Light Screen, Reflect, etc.
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (type == "-supereffective") {
		// message.remainder() == POKEMON
	} else if (type == "switch" or type == "drag") {
		auto const parsed = parse_switch(message);

		if (m_battle.is_me(parsed.party)) {
			auto const index = get_species_index(m_battle.ai(), parsed.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		constexpr auto slot = 0;
		auto const move = m_battle.find_or_add_pokemon(parsed.party, slot, parsed.species, parsed.level, parsed.gender);
		if (type == "drag") {
			m_move_state.phaze_index(other(parsed.party), get_team(m_battle, parsed.party), parsed.species);
		} else {
			m_move_state.use_move(parsed.party, move);
		}
	} else if (type == "replace") {
#if 0
		// Illusion ended
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (type == "-singleturn") {
		// Received for things like Protect that last the rest of the turn
	} else if (type == "-start") {
		auto const pokemon_id [[maybe_unused]] = message.next();
		auto const thing = message.next();
		if (thing == "confusion") {
			auto const source = message.next();
			if (source == "[fatigue]") {
				
			} else {
				m_move_state.confuse();
			}
		}
	} else if (type == "-status") {
		auto const party = party_from_player_id(message.next());
		auto const status = parse_status(message.next());
		auto const source = parse_hp_change_source(message);
		bounded::visit(source, bounded::overload(
			[&](MainEffect) { m_move_state.status(party, status); },
			[](FromConfusion) { throw std::runtime_error("Confusion cannot cause another status"); },
			[&](FromMiscellaneous) { m_move_state.status(other(party), status); },
			[&](FromMove) { m_move_state.status(party, status); },
			[&](auto const value) { m_battle.set_value_on_active(party, value); }
		));
	} else if (type == "swap") {
#if 0
		auto const pokemon = message.next();
		auto const position = message.next();
		// position here is "0", "1", or "2"
#endif
	} else if (type == "tie") {
		m_completed = true;
	} else if (type == "-transform") {
		// message.remainder() == POKEMON|SPECIES
	} else if (type == "turn") {
		auto const turn = bounded::to_integer<0, std::numeric_limits<std::uint32_t>::max()>(message.next());
		m_analysis_logger << std::string(20, '=') << "\nBegin turn " << turn << '\n';
		m_battle.handle_begin_turn(turn);
		send_move(determine_action());
	} else if (type == "-unboost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (type == "upkeep") {
	} else if (type == "-weather") {
#if 0
		auto const weather = message.next();
#endif
	} else if (type == "win") {
		if (m_log_foe_teams) {
			auto const won = m_username == message.next();
			if (!won) {
				log_foe_team(m_usage_stats, m_battle.foe(), m_random_engine);
			}
		}
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
	}
}

void BattleParser::handle_damage(InMessage message) {
	auto const parsed = parse_hp_message(message);
	auto move_damage = [&](Party const party) {
		m_move_state.damage(party, {parsed.current_hp, parsed.max_hp, parsed.status});
	};
	auto const party = parsed.party;
	bounded::visit(parsed.source, bounded::overload(
		[&](MainEffect) {
			if (m_move_state.move_damages_self(party)) {
				return;
			}
			move_damage(other(party));
			if (m_battle.is_me(other(party)) and m_move_state.executed_move() == Moves::U_turn) {
				maybe_use_previous_move();
				send_move(determine_action());
			}
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
		[&](auto const value) { m_battle.set_value_on_active(party, value); }
	));
}

namespace {

auto to_real_hp(bool const is_me, Pokemon const & changer, HP::current_type const remaining_visible_hp, HP::max_type const max_visible_hp) -> HP::current_type {
	if (remaining_visible_hp == 0_bi) {
		return 0_bi;
	}
	if (remaining_visible_hp > max_visible_hp) {
		throw std::runtime_error("Received a current HP greater than max HP");
	}
	constexpr auto max_visible_foe_hp = 100_bi;
	auto const max_hp = get_hp(changer).max();
	auto const expected_max_visible_hp = BOUNDED_CONDITIONAL(is_me, max_hp, max_visible_foe_hp);
	if (expected_max_visible_hp != max_visible_hp) {
		throw std::runtime_error("Received an invalid max HP. Expected: " + bounded::to_string(expected_max_visible_hp) + " but got " + bounded::to_string(max_visible_hp));
	}
	return HP::current_type(bounded::max(1_bi, max_hp * remaining_visible_hp / max_visible_hp));
}

auto hp_to_damage(Pokemon const & pokemon, HP::current_type const new_hp) {
	auto const old_hp = get_hp(pokemon).current();
	if (new_hp > old_hp) {
		throw std::runtime_error("Took negative damage");
	}
	return static_cast<damage_type>(old_hp - new_hp);
}

auto const & select_pokemon(Team const & team, Moves const move) {
	return is_switch(move) ? team.pokemon(to_replacement(move)) : team.pokemon();
}

auto compute_damage(Battle const & battle, Party const user_party, Moves const move, MoveState::Damage const damage) {
	auto const damaged_party = move == Moves::Hit_Self ? user_party : other(user_party);
	auto const is_me = battle.is_me(damaged_party);
	auto const & team = is_me ? battle.ai() : battle.foe();
	auto const & pokemon = select_pokemon(team, move);
	auto const new_hp = to_real_hp(is_me, pokemon, damage.current_hp, damage.max_hp);
	return hp_to_damage(pokemon, new_hp);
}

} // namespace

void BattleParser::maybe_use_previous_move() {
	auto const maybe_data = m_move_state.complete();
	if (!maybe_data) {
		return;
	}
	auto const data = *maybe_data;
	constexpr auto slot = 0;
	// TODO: This should never be bounded::none, just 0.
	auto const damage = BOUNDED_CONDITIONAL(
		data.damage,
		compute_damage(m_battle, data.party, data.move.executed, *data.damage),
		bounded::none
	);
	
	auto const other_pokemon = get_team(m_battle, data.party).pokemon();
	auto const other_move = other_pokemon.moved() ?
		OtherMove(other_pokemon.last_used_move().name()) :
		OtherMove(FutureMove{(data.move.executed == Moves::Sucker_Punch) and damage and *damage != 0_bi});

	m_battle.handle_use_move(data.party, slot, data.move, data.variable, data.miss, data.critical_hit, data.clear_status, damage, other_move);
}

Moves BattleParser::determine_action() {
	if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}

	m_analysis_logger << to_string(m_battle.ai()) << '\n';
	m_analysis_logger << "Seen " << to_string(m_battle.foe()) << '\n';
	auto predicted = predict_team(m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
	m_analysis_logger << "Predicted " << to_string(predicted) << '\n' << std::flush;

	return expectiminimax(m_battle.ai(), predicted, m_battle.weather(), m_evaluate, Depth(m_depth, 1U), m_analysis_logger);
}

void BattleParser::send_move(Moves const move) {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	using std::to_string;
	auto switch_move = [&]{ return to_string(m_slot_memory[to_replacement(move)]); };
	auto move_index = [&]{ return to_string(get_move_index(m_battle.ai().pokemon(), move) + 1_bi); };
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
	auto const party = party_from_player_id(message.next(": "));
	auto const nickname [[maybe_unused]] = message.next();
	auto const details = parse_details(message.next());
	auto const hp = message.next(' ');
	static_cast<void>(hp);
	auto const status = parse_status(message.next());
	return ParsedSwitch{party, details.species, details.level, details.gender, status};
}

}	// namespace ps
}	// namespace technicalmachine
