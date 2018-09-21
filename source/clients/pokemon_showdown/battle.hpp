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

#pragma once

#include "inmessage.hpp"
#include "json_parser.hpp"

#include "../battle.hpp"

#include <bounded/optional.hpp>
#include <containers/vector/vector.hpp>

#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
namespace ps {

struct BattleParser {
	BattleParser(
		boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket,
		std::string id_,
		std::string username,
		DetailedStats const & detailed,
		Evaluate evaluate,
		Party party,
		std::string opponent,
		unsigned depth,
		std::mt19937 random_engine,
		Team team,
		TeamSize opponent_team_size
	):
		m_websocket(websocket),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_battle(
			detailed,
			evaluate,
			party,
			std::move(opponent),
			depth,
			random_engine,
			std::move(team),
			opponent_team_size,
			100_bi
		)
	{
		for (auto const index : containers::integer_range(opponent_team_size)) {
			push_back(m_slot_memory, index + 1_bi);
		}
	}
	
	void handle_message(InMessage message);

	std::string_view id() const {
		return m_id;
	}	
	bool completed() const {
		return m_completed;
	}
private:
	void send_move(Moves const move);
	void send_random_move();
	void send_message(std::string_view message);

	boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & m_websocket;
	std::string m_id;
	std::string m_username;
	Battle m_battle;
	using ShowdownIndex = decltype(std::declval<containers::index_type<PokemonContainer>>() + 1_bi);
	containers::static_vector<ShowdownIndex, max_pokemon_per_team.value()> m_slot_memory;
	bool m_completed = false;
};


struct BattleFactory {
	BattleFactory(std::string id_, std::string username, DetailedStats const & detailed, Evaluate evaluate, unsigned depth, std::mt19937 random_engine, Team team):
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_detailed(detailed),
		m_evaluate(evaluate),
		m_depth(depth),
		m_random_engine(random_engine),
		m_team(std::move(team))
	{
		std::cout << "Starting a battle with id " << m_id << '\n';
	}
	
	std::string_view id() const {
		return m_id;
	}
	
	void handle_message(InMessage message);
	
	bool completed() const {
		return m_completed;
	}
	bounded::optional<BattleParser> make(boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket) &&;

private:
	enum class Clause { };
	JSONParser m_parse_json;

	std::string m_id;
	std::string m_username;
	DetailedStats const & m_detailed;
	Evaluate m_evaluate;
	unsigned m_depth;
	std::mt19937 m_random_engine;
	Team m_team;
	containers::vector<Clause> m_rules;
	bounded::optional<std::string> m_opponent;
	bounded::optional<std::string> m_player_id;
	bounded::optional<std::string> m_type;	// singles, doubles, triples
	bounded::optional<std::string> m_tier;
	bounded::optional<bounded::integer<1, 7>> m_generation;
	bounded::optional<TeamSize> m_opponent_team_size;
	bool m_completed = false;
};

}	// namespace ps
}	// namespace technicalmachine
