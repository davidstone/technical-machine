// Pokemon Showdown battle logic
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

#include "battle.hpp"

#include "../../string_conversions/pokemon.hpp"

#include <bounded/integer.hpp>

#include <charconv>
#include <iostream>

namespace technicalmachine {
namespace ps {

namespace {

constexpr auto make_party(std::string_view player_id) {
	return 	Party(BOUNDED_CONDITIONAL(player_id == "p1", 0_bi, 1_bi));
}

constexpr auto party_from_pokemon_id(std::string_view const player_id) {
	return make_party(player_id.substr(0, 2));
}

template<auto minimum, auto maximum, typename policy = bounded::throw_policy<>>
constexpr auto to_integer(std::string_view const str) {
	auto result = bounded::integer<0, maximum, policy>(0_bi);
	for (auto const c : str) {
		result *= 10_bi;
		// TODO: throw an exception type that mentions a bad character?
		result += bounded::checked_integer<0, 9>(c - '0');
	}
	return bounded::integer<minimum, maximum, policy>(result);
}

// This adds additional string scanning, but I don't think the performance
// matters that much here
auto parse_details(std::string_view details) {
	auto parser = BufferView(details, ", ");
	auto const species = from_string<Species>(parser.next());
	auto const level = Level(to_integer<Level::min, Level::max>(parser.next().substr(1)));
	auto const gender_str = parser.next();
	auto const gender =
		(gender_str == "F") ? Gender::female :
		(gender_str == "M") ? Gender::male :
		(gender_str == "") ? Gender::genderless :
		throw std::runtime_error("Invalid gender string " + std::string(gender_str));
	struct result {
		Species species;
		Level level;
		Gender gender;
	};
	return result{species, level, gender};
}

bounded::optional<Team> parse_team(boost::property_tree::ptree const & pt) {
	try {
		auto const team_data = range_view(pt.get_child("side").get_child("pokemon").equal_range(""));
		constexpr bool is_me = true;
		Team team(TeamSize(containers::distance(team_data.begin(), team_data.end())), is_me);
		for (auto const & pokemon_data : team_data) {
			auto get = [&](auto const & key) { return pokemon_data.second.get<std::string>(key); };

			auto const details = parse_details(get("details"));
			
			auto const condition = get("condition");
			// current_hp/max_hp
			// Presumably also gives me status information? Should be useful for
			// rejoining battles
			static_cast<void>(condition);
			
			auto const stats = get("stats");
			// Need to turn this into EVs somehow
			// {"atk":214,"def":147,"spa":197,"spd":147,"spe":180}
			static_cast<void>(stats);
			
			auto const ability = from_string<Ability>(get("baseAbility"));
			
			auto const item = from_string<Item>(get("item"));
			
			team.add_pokemon(details.species, details.level, details.gender, item, ability, Nature::Hardy);
			
			Pokemon & pokemon = team.replacement();
			for (auto const & move : pokemon_data.second.get_child("moves")) {
				 add_seen_move(all_moves(pokemon), from_string<Moves>(move.second.get<std::string>("")));
			}
			team.all_pokemon().reset_index();
		}
		return team;
	} catch (std::exception const & ex) {
		std::cerr << "Unable to parse team: " << ex.what() << '\n';
		return bounded::none;
	}
}

}	// namespace

void BattleFactory::handle_message(InMessage message) {
	// Documented at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md
	// under the section "Battle Initialization"
	if (message.type() == "clearpoke") {
		// This appears to mean nothing
	} else if (message.type() == "gametype") {
		m_type.emplace(message.next());
	} else if (message.type() == "gen") {
		m_generation.emplace(to_integer<1, 7>(message.next()));
	} else if (message.type() == "player") {
		auto const player_id = message.next();
		auto const username = message.next();
		if (username == m_username) {
			m_player_id.emplace(player_id);
		} else {
			m_opponent.emplace(username);
		}
		// message.remainder() == AVATAR
	} else if (message.type() == "poke") {
		std::cout << "poke: " << message.remainder() << '\n';
		// message.remainder() == PLAYER_ID|DETAILS|ITEM
	} else if (message.type() == "rated") {
		// Received if and only if the game is rated. We don't care about this
	} else if (message.type() == "request") {
		// It seems that the game starts off with a single "request" that has
		// no data. Then it sends another request with data, which we need to
		// parse for Random Battles.

		auto const json_data = message.remainder();
		if (!json_data.empty()) {
			if (auto team = parse_team(m_parse_json(json_data))) {
				m_team = *std::move(team);
			}
		}
	} else if (message.type() == "rule") {
		// message.remainder() == RULE: DESCRIPTION
		// Received for each clause in effect
	} else if (message.type() == "seed") {
		// I have no idea what this is
	} else if (message.type() == "start") {
		m_completed = true;
	} else if (message.type() == "teampreview") {
		// This appears to mean nothing
	} else if (message.type() == "teamsize") {
		auto const player_id = message.next();
		if (!m_player_id) {
			std::cerr << "Received a teamsize message before receiving a player id.\n";
		}
		if (*m_player_id == player_id) {
			m_opponent_team_size.emplace(to_integer<0, 6>(message.next()));
		}
	} else if (message.type() == "tier") {
		m_tier.emplace(message.next());
	} else if (message.type() == "title") {
		// message.remainder() == PLAYER1 vs. PLAYER2
	} else {
		std::cerr << "Received battle setup message of unknown type: " << message.type() << ": " << message.remainder() << '\n';
	}
}

bounded::optional<BattleParser> BattleFactory::make(boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket) && {
	assert(m_completed);
	if (!m_opponent) {
		std::cerr << "Did not receive opponent\n";
		return bounded::none;
	}
	if (!m_player_id) {
		std::cerr << "Did not receive player_id\n";
		return bounded::none;
	}
	if (!m_type) {
		std::cerr << "Did not receive battle format\n";
		return bounded::none;
	}
	if (!m_tier) {
		std::cerr << "Did not receive tier\n";
		return bounded::none;
	}
	if (!m_generation) {
		std::cerr << "Did not receive generation\n";
		return bounded::none;
	}
	if (!m_opponent_team_size) {
		std::cerr << "Did not receive opponent team size\n";
		return bounded::none;
	}
	if (*m_type != "singles") {
		std::cerr << "Unsupported format " << *m_type << '\n';
		return bounded::none;
	}
	if (*m_generation != 4) {
		std::cerr << "Unsupported generation " << *m_generation << '\n';
		return bounded::none;
	}
	if (*m_player_id != "p1" and *m_player_id != "p2") {
		std::cerr << "Invalid player id: " << *m_player_id << '\n';
		return bounded::none;
	}
	return BattleParser(
		websocket,
		std::move(m_id),
		std::move(m_username),
		m_overall,
		m_detailed,
		m_lead,
		m_multiplier,
		m_evaluate,
		make_party(*m_player_id),
		*std::move(m_opponent),
		m_depth,
		m_random_engine,
		std::move(m_team),
		*m_opponent_team_size
	);
}


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
		m_battle.handle_critical_hit(other(party_from_pokemon_id(message.next())));
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
		auto const hp = message.next(' ');
		auto const status = message.next();
		static_cast<void>(party);
		std::cout << "-damage: hp: " << hp << " status: " << status << " remainder: " << message.remainder() << '\n';
//		m_battle.handle_direct_damage(party, 0, 
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
		if (m_battle.is_me(party)) {
			m_replacing_fainted = true;
			send_move(m_battle.determine_action(m_random_engine));
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
		m_battle.handle_use_move(party, slot, move);
	} else if (message.type() == "player") {
		// At the end of a battle, I received this with a body of "p1|"
	} else if (message.type() == "request") {
		std::cout << "request: " << message.remainder() << '\n';
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
		auto const pokemon = message.next();
		auto const details = parse_details(message.next());
		auto const hp = message.next(' ');
		static_cast<void>(hp);
		auto const status = message.next();
		static_cast<void>(status);
		
		if (m_battle.is_me(party)) {
			auto const index = m_battle.species_index(details.species);
			if (m_replacing_fainted) {
				m_slot_memory.replace_fainted(index);
				m_replacing_fainted = false;
			} else {
				m_slot_memory.switch_to(index);
			}
		}

		constexpr auto slot = 0;
		m_battle.handle_send_out(party, slot, std::string(pokemon), details.species, details.gender, details.level);
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
		m_battle.handle_end(Result::tied, m_random_engine);
		m_completed = true;
	} else if (message.type() == "-transform") {
		// message.remainder() == POKEMON|SPECIES
	} else if (message.type() == "turn") {
		auto const turn = to_integer<0, std::numeric_limits<std::uint32_t>::max()>(message.next());
		m_battle.handle_begin_turn(turn);
		send_move(m_battle.determine_action(m_random_engine));
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
		m_battle.handle_end(won ? Result::won : Result::lost, m_random_engine);
		m_completed = true;
	} else {
		std::cerr << "Received battle progress message of unknown type: " << message.type() << ": " << message.remainder() << '\n';
	}
}

void BattleParser::send_move(Moves const move) {
	// In doubles / triples we need to specify " TARGET" at the end for regular
	// moves
	using std::to_string;
	auto switch_move = [&]{ return to_string(m_slot_memory[to_replacement(move)]); };
	auto move_index = [&]{ return to_string(m_battle.move_index(move) + 1_bi); };
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
