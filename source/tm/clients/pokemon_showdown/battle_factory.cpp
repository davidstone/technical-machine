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

#include <tm/clients/pokemon_showdown/battle_factory.hpp>

#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <bounded/integer.hpp>

#include <charconv>
#include <iostream>

namespace technicalmachine {
namespace ps {

namespace {

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
			
			Pokemon & pokemon = back(team.all_pokemon());
			for (auto const & move : pokemon_data.second.get_child("moves")) {
				 add_seen_move(all_moves(pokemon), from_string<Moves>(move.second.get<std::string>("")));
			}
			set_hp_ev(pokemon, EV(80_bi));
			for (auto const stat : {StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE}) {
				set_stat_ev(pokemon, stat, EV(84_bi));
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
		m_generation.emplace(bounded::to_integer<1, 7>(message.next()));
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
			std::cout << json_data << '\n';
			if (auto team = parse_team(m_parse_json(json_data))) {
				// TODO: maybe just do this on random battles, since we know
				// what our team is otherwise?
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
			m_opponent_team_size.emplace(bounded::to_integer<TeamSize>(message.next()));
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
		m_usage_stats,
		m_evaluate,
		make_party(*m_player_id),
		*std::move(m_opponent),
		m_depth,
		m_random_engine,
		std::move(m_team),
		*m_opponent_team_size
	);
}

}	// namespace ps
}	// namespace technicalmachine
