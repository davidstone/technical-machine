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

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/overload.hpp>

#include <containers/algorithms/find.hpp>
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

constexpr auto party_from_pokemon_id(std::string_view const player_id) {
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

auto to_real_hp(bool const is_me, Pokemon const & changer, std::string_view const remaining_visible_hp_str, std::string_view const max_visible_hp_str) -> HP::current_type {
	auto const remaining_visible_hp = bounded::to_integer<HP::current_type>(remaining_visible_hp_str);
	if (remaining_visible_hp == 0_bi) {
		return 0_bi;
	}
	auto const max_visible_hp = bounded::to_integer<HP::current_type>(max_visible_hp_str);
	if (remaining_visible_hp > max_visible_hp) {
		throw std::runtime_error("Received a current HP greater than max HP");
	}
	constexpr auto max_visible_foe_hp = 100_bi;
	auto const max_hp = get_hp(changer).max();
	auto const expected_max_visible_hp = BOUNDED_CONDITIONAL(is_me, max_hp, max_visible_foe_hp);
	if (expected_max_visible_hp != max_visible_hp) {
		throw std::runtime_error("Received an invalid max HP");
	}
	return HP::current_type(bounded::max(1_bi, max_hp * remaining_visible_hp / max_visible_hp));
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

auto parse_hp_and_status(std::string_view const hp_and_status, Battle const & battle, Party const party) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	// TODO: validate that the status we received matches our expected status
	struct Result {
		HP::current_type hp;
		Statuses status;
	};
	auto const [hp_fraction, status] = split(hp_and_status, ' ');
	auto const [remaining_visible_hp, max_visible_hp] = split(hp_fraction, '/');
	Pokemon const & pokemon = get_team(battle, party).pokemon();
	return Result{
		to_real_hp(battle.is_me(party), pokemon, remaining_visible_hp, max_visible_hp),
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

// This does work that we do not always need, but it should not matter in the
// scheme of handling websocket messages.
auto parse_hp_message(InMessage message, Battle const & battle) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct Message {
		Party party;
		HP::current_type hp;
		Statuses status;
		HPChangeSource source;
	};
	auto const party = party_from_pokemon_id(message.next());
	auto const hp_and_status = parse_hp_and_status(message.next(), battle, party);
	auto const source = parse_hp_change_source(message);
	return Message{
		party,
		hp_and_status.hp,
		hp_and_status.status,
		source
	};
}

auto parse_set_hp_message(InMessage message, Battle const & battle) {
	// TODO: This should just verify that the received HP matches the actual
	// HP. The problem is that we tend to get this message too soon, so I need
	// to defer checking until some time later.
	struct MessagePokemon {
		Party party;
		HP::current_type hp;
		Statuses status;
	};
	struct Message {
		MessagePokemon pokemon1;
		MessagePokemon pokemon2;
		HPChangeSource source;
	};
	auto const party1 = party_from_pokemon_id(message.next());
	auto const hp_and_status1 = parse_hp_and_status(message.next(), battle, party1);
	auto const party2 = party_from_pokemon_id(message.next());
	auto const hp_and_status2 = parse_hp_and_status(message.next(), battle, party2);
	auto const source = parse_hp_change_source(message);
	
	return Message{
		{party1, hp_and_status1.hp, hp_and_status1.status},
		{party2, hp_and_status2.hp, hp_and_status2.status},
		source
	};
}

} // namespace


void BattleParser::handle_message(InMessage message) {
	m_battle_logger.log(message);
	
	if (handle_chat_message(message)) {
		return;
	}

	struct PokemonDetails {
		Species species;
		bool shiny; // "shiny" or nothing
		Gender gender; // "M", "F", or nothing
		Level level; // Level 100 by default
	};

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
		auto const party = party_from_pokemon_id(message.next());
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
		auto const user = other(party_from_pokemon_id(message.next()));
		m_move_state.critical_hit(user);
	} else if (type == "-curestatus") {
		auto const party = party_from_pokemon_id(message.next());
		auto const status = message.next();
		if (status == "slp") {
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
		auto const pokemon = message.next();
		auto const action = message.next();
		// Depending on what `action` is, there could be more
#endif
	} else if (type == "faint") {
		constexpr auto slot = 0;
		auto const party = party_from_pokemon_id(message.next());
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
		auto const parsed = parse_hp_message(message, m_battle);
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
	} else if (type == "-immune") {
		auto const party = party_from_pokemon_id(message.next());
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
		auto const party = party_from_pokemon_id(message.next());
		auto const item = from_string<Item>(message.next());
		m_battle.set_value_on_active(party, item);
	} else if (type == "-mega") {
#if 0
		auto const pokemon = message.next();
		auto const megastone = message.next();
#endif
	} else if (type == "-message") {
	} else if (type == "-miss") {
		auto const user_party = party_from_pokemon_id(message.next());
		m_move_state.miss(user_party);
	} else if (type == "move") {
		auto const party = party_from_pokemon_id(message.next());
		auto const move = from_string<Moves>(message.next());
#if 0
		// target is sent only for moves that target one Pokemon
		auto const target = message.next();
#endif
		auto const previous_move_party = m_move_state.party();
		if (previous_move_party == party) {
			m_move_state.use_executed_move(move);
		}
		maybe_use_previous_move();
		m_move_state.use_move(party, move);
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
		auto const parsed = parse_set_hp_message(message, m_battle);
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
		if (type == "drag") {
			auto const phazer_party = other(parsed.party);
			m_move_state.phaze_index(phazer_party, get_team(m_battle, parsed.party), parsed.species);
			m_battle.add_pokemon_from_phaze(parsed.party, slot, parsed.species, parsed.level, parsed.gender);
		} else {
			m_battle.handle_send_out(parsed.party, slot, parsed.species, parsed.level, parsed.gender);
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
		} else {
			std::cout << "Miscellaneous -start effect: " << thing << '|' << message.remainder() << '\n';
		}
	} else if (type == "-status") {
		auto const party = party_from_pokemon_id(message.next());
		auto const status = parse_status(message.next());
		auto const source = parse_hp_change_source(message);
		bounded::visit(source, bounded::overload(
			[](MainEffect) {},
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
		handle_battle_end(Result::tied, m_usage_stats, m_battle.foe(), m_random_engine);
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
		auto const won = m_username == message.next();
		auto const result = won ? Result::won : Result::lost;
		handle_battle_end(result, m_usage_stats, m_battle.foe(), m_random_engine);
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << type << ": " << message.remainder() << '\n';
	}
}

void BattleParser::handle_damage(InMessage message) {
	auto const parsed = parse_hp_message(message, m_battle);
	auto move_damage = [&](Party const damaged_party, Party const user_party) {
		auto const old_hp = get_hp(get_team(m_battle, damaged_party).pokemon()).current();
		auto const new_hp = parsed.hp;
		if (new_hp > old_hp) {
			throw std::runtime_error("Took negative damage");
		}
		m_move_state.damage(user_party, static_cast<damage_type>(old_hp - new_hp));
	};
	auto const party = parsed.party;
	bounded::visit(parsed.source, bounded::overload(
		[&](MainEffect) {
			if (m_move_state.move_damages_self(party)) {
				return;
			}
			move_damage(party, other(party));
		},
		[&](FromConfusion) {
			// TODO: Technically you cannot select Hit Self, you just execute
			// it. This matters for things like priority or determining whether
			// Sucker Punch succeeds.
			m_move_state.use_move(party, Moves::Hit_Self);
			move_damage(party, party);
		},
		[](FromMiscellaneous) {},
		[](FromMove) {},
		[&](auto const value) { m_battle.set_value_on_active(party, value); }
	));
}

void BattleParser::maybe_use_previous_move() {
	if (auto const maybe_data = m_move_state.complete()) {
		auto const data = *maybe_data;
		constexpr auto slot = 0;
		m_battle.handle_use_move(data.party, slot, data.move, data.variable, data.miss, data.critical_hit, data.clear_status, data.damage);
	}
}

Moves BattleParser::determine_action() {
	if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}

	m_analysis_logger << to_string(m_battle.ai()) << '\n';
	m_analysis_logger << "Seen " << to_string(m_battle.foe()) << '\n';
	auto predicted = predict_team(m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
	m_analysis_logger << "Predicted " << to_string(predicted) << '\n';

	return expectiminimax(m_battle.ai(), predicted, m_battle.weather(), m_evaluate, Depth(m_depth, 0U), m_analysis_logger);
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
	auto const party = party_from_pokemon_id(message.next(": "));
	auto const nickname [[maybe_unused]] = message.next();
	auto const details = parse_details(message.next());
	auto const hp = message.next(' ');
	static_cast<void>(hp);
	auto const status = parse_status(message.next());
	return ParsedSwitch{party, details.species, details.level, details.gender, status};
}

}	// namespace ps
}	// namespace technicalmachine
