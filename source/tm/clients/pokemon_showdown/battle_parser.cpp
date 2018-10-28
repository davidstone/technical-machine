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

#include <tm/evaluate/expectiminimax.hpp>

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/integer.hpp>

#include <containers/algorithms/find.hpp>
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

} // namespace


void BattleParser::handle_message(InMessage message) {
	std::cout << "Message: " << message.type() << '\n';
	struct PokemonDetails {
		Species species;
		bool shiny; // "shiny" or nothing
		Gender gender; // "M", "F", or nothing
		Level level; // Level 100 by default
	};

	// POKEMON = PLAYER+SLOT: NAME
	// Where PLAYER is "p1" or "p2", SLOT is "a", "b", or "c". Example: p1a: Mew
	// SLOT is left off for inactive Pokemon

	// Documented at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
	// under the section "Battle progress"
	if (message.type() == "-ability") {
		try {
			auto const party = party_from_pokemon_id(message.next());
			auto const ability = from_string<Ability>(message.next());
			m_battle.handle_ability_message(party, ability);
		} catch (std::exception const & ex) {
			std::cerr << "Error in handling ability message: " << ex.what() << '\n';
		}
	} else if (message.type() == "-activate") {
		std::cout << "Miscellaneous effect: " << message.remainder() << '\n';
	} else if (message.type() == "-boost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (message.type() == "cant") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == "REASON" or "REASON|MOVE"
#endif
	} else if (message.type() == "-center") {
	} else if (message.type() == "-crit") {
		// TODO: Is this the user or the target?
		//auto const user = other(party_from_pokemon_id(message.next()));
	} else if (message.type() == "-curestatus") {
#if 0
		auto const pokemon = message.next();
		auto const status = message.next();
#endif
	} else if (message.type() == "-cureteam") {
#if 0
		auto const pokemon = message.next();
#endif
	} else if (message.type() == "-damage") {
		auto const party = party_from_pokemon_id(message.next());
		auto const hp_and_status = message.next();
		// `from` is "[from]" or empty.
		auto const from [[maybe_unused]] = message.next(' ');
		auto const source = message.next(':');
		auto const item = message.next();
		// from might be empty, might say it's from a status, and might say it
		// is from Life Orb. This is the only indication we get that the user
		// has Life Orb.
		
		auto parse_hp = [=]{
			auto const hp_str = std::string_view(hp_and_status.data(), hp_and_status.find_first_of(" /"));
			return bounded::to_integer<VisibleHP>(hp_str);
		};
		
		constexpr auto slot = 0;
		m_battle.handle_hp_change(party, slot, parse_hp());
		
		if (source == "item") {
			m_battle.handle_item_message(party, from_string<Item>(item));
		}
		
	} else if (message.type() == "deinit") {
		// When you stay in a room too long
	} else if (message.type() == "detailschange" or message.type() == "-formechange") {
#if 0
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (message.type() == "-end") {
		// Tells me if the substitute is destroyed, probably other stuff ending
		// message.remainder() == PLAYER: POKEMON|EFFECT
	} else if (message.type() == "-endability") {
#if 0
		auto const pokemon = message.next();
		auto const ability = message.next();
#endif
	} else if (message.type() == "-enditem") {
#if 0
		auto const pokemon = message.next();
		auto const item = message.next();
#endif
	} else if (message.type() == "error") {
		std::cerr << message.remainder() << '\n';
		if (message.remainder() != "[Invalid choice] There's nothing to choose") {
			send_random_move();
		}
	} else if (message.type() == "expire") {
		// When you stay in a room too long
	} else if (message.type() == "-fail") {
#if 0
		auto const pokemon = message.next();
		auto const action = message.next();
		// Depending on what `action` is, there could be more
#endif
	} else if (message.type() == "faint") {
		constexpr auto slot = 0;
		auto const party = party_from_pokemon_id(message.next());
		m_battle.handle_fainted(party, slot);
		if (m_battle.is_me(party) and (m_battle.ai().size() != 1_bi or get_hp(m_battle.ai().pokemon()) != 0_bi)) {
			m_replacing_fainted = true;
			send_move(determine_action());
		}
	} else if (message.type() == "-fieldend") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
#endif
	} else if (message.type() == "-fieldstart") {
#if 0
		// Grassy Terrain, Gravity, Trick Room
		auto const condition = message.next();
#endif
	} else if (message.type() == "-heal") {
#if 0
		auto const pokemon = message.next();
		auto const hp = message.next(' ');
		auto const status = message.next();
#endif
	} else if (message.type() == "-hint") {
		// message.remainder() == MESSAGE
	} else if (message.type() == "-immune") {
		// message.remainder() == POKEMON
	} else if (message.type() == "inactive") {
		// message.remainder() == MESSAGE
		// Timer is on
	} else if (message.type() == "inactiveoff") {
		// message.remainder() == MESSAGE
		// Timer is off
	} else if (message.type() == "-item") {
		auto const party = party_from_pokemon_id(message.next());
		auto const item = from_string<Item>(message.next());
		m_battle.handle_item_message(party, item);
	} else if (message.type() == "-mega") {
#if 0
		auto const pokemon = message.next();
		auto const megastone = message.next();
#endif
	} else if (message.type() == "-message") {
		std::cout << "Battle message: " << message.remainder() << '\n';
	} else if (message.type() == "move") {
		auto const party = party_from_pokemon_id(message.next());
		constexpr auto slot = 0;
		auto const move = from_string<Moves>(message.next());
#if 0
		// target is sent only for moves that target one Pokemon
		auto const target = message.next();
#endif
		if (is_phaze(move)) {
			if (m_phaze_move) {
				throw std::runtime_error("Recieved data for next move before receiving phaze data.");
			}
			m_phaze_move.emplace(party, move);
		} else {
			m_battle.handle_use_move(party, slot, move);
		}
	} else if (message.type() == "player") {
		// At the end of a battle, I received this with a body of "p1|"
	} else if (message.type() == "request") {
#if 0
		auto const json = message.remainder();
#endif
	} else if (message.type() == "-resisted") {
		// message.remainder() == POKEMON
	} else if (message.type() == "-sideend") {
#if 0
		// Light Screen, Reflect, etc.
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (message.type() == "-sidestart") {
#if 0
		// Light Screen, Reflect, etc.
		auto const side = message.next();
		auto const condition = message.next();
#endif
	} else if (message.type() == "-supereffective") {
		// message.remainder() == POKEMON
	} else if (message.type() == "switch" or message.type() == "drag") {
		auto const party = party_from_pokemon_id(message.next(": "));
		auto const nickname [[maybe_unused]] = message.next();
		auto const details = parse_details(message.next());
		auto const hp = message.next(' ');
		static_cast<void>(hp);
		auto const status = message.next();
		static_cast<void>(status);
		
		if (m_battle.is_me(party)) {
			auto const index = get_species_index(m_battle.ai(), details.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		constexpr auto slot = 0;
		if (m_phaze_move) {
			if (message.type() != "drag") {
				throw std::runtime_error("Never received phaze data.");
			}
			if (party == m_phaze_move->party) {
				throw std::runtime_error("Received phaze data for the wrong team.");
			}
			m_battle.handle_phaze(m_phaze_move->party, slot, slot, m_phaze_move->move, details.species, details.level, details.gender);
			m_phaze_move = bounded::none;
		} else {
			m_battle.handle_send_out(party, slot, details.species, details.level, details.gender);
		}
	} else if (message.type() == "replace") {
#if 0
		// Illusion ended
		auto const pokemon = message.next();
		// message.remainder() == DETAILS|HP STATUS
#endif
	} else if (message.type() == "-singleturn") {
		// Received for things like Protect that last the rest of the turn
	} else if (message.type() == "-start") {
		// This should not be necessary, and it is in a weird format
		// This at least tells me about the start of a substitute
	} else if (message.type() == "-status") {
#if 0
		auto const pokemon = message.next();
		auto const status = message.next();
#endif
	} else if (message.type() == "swap") {
#if 0
		auto const pokemon = message.next();
		auto const position = message.next();
		// position here is "0", "1", or "2"
#endif
	} else if (message.type() == "tie") {
		handle_battle_end(Result::tied, m_battle.foe_name(), m_usage_stats, m_battle.foe(), m_random_engine);
		m_completed = true;
	} else if (message.type() == "-transform") {
		// message.remainder() == POKEMON|SPECIES
	} else if (message.type() == "turn") {
		auto const turn = bounded::to_integer<0, std::numeric_limits<std::uint32_t>::max()>(message.next());
		m_battle.handle_begin_turn(turn);
		send_move(determine_action());
	} else if (message.type() == "-unboost") {
#if 0
		auto const pokemon = message.next();
		auto const stat = message.next();
		auto const amount = message.next();
#endif
	} else if (message.type() == "upkeep") {
	} else if (message.type() == "-weather") {
#if 0
		auto const weather = message.next();
#endif
	} else if (message.type() == "win") {
		auto const won = m_username == message.next();
		auto const result = won ? Result::won : Result::lost;
		handle_battle_end(result, m_battle.foe_name(), m_usage_stats, m_battle.foe(), m_random_engine);
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << message.type() << ": " << message.remainder() << '\n';
	}
}

Moves BattleParser::determine_action() {
	if (m_battle.ai().size() == 0_bi or m_battle.foe().size() == 0_bi) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}

	std::cout << std::string(20, '=') + '\n';
	std::cout << "Predicting...\n";
	auto predicted = predict_team(m_usage_stats, use_lead_stats, m_battle.foe(), m_random_engine);
	//std::cout << to_string(predicted) << '\n';

	return expectiminimax(m_battle.ai(), predicted, m_battle.weather(), m_evaluate, Depth(m_depth, 1U));
}

void BattleParser::send_move(Moves const move) {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	using std::to_string;
	auto switch_move = [&]{ return to_string(m_slot_memory[to_replacement(move)]); };
	auto move_index = [&]{ return to_string(get_move_index(m_battle.ai().pokemon(), move) + 1_bi); };
	send_message(m_id + (is_switch(move) ? "|/switch " + switch_move() : "|/move " + move_index()));
}

void BattleParser::send_random_move() {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	auto distribution = std::uniform_int_distribution(1, static_cast<int>(max_moves_per_pokemon + max_pokemon_per_team));
	auto const result = distribution(m_random_engine);

	auto switch_move = [=]{ return std::to_string(result - max_moves_per_pokemon); };
	auto move_index = [=]{ return std::to_string(result); };
	auto const is_switch = result > max_moves_per_pokemon;
	send_message(m_id + (is_switch ? "|/switch " + switch_move() : "|/move " + move_index()));
}

void BattleParser::send_message(std::string_view const message) {
	m_websocket.write(boost::asio::buffer(message));
}

}	// namespace ps
}	// namespace technicalmachine
