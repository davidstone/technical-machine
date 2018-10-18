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
#include "slot_memory.hpp"

#include "../battle.hpp"

#include <bounded/optional.hpp>
#include <containers/vector.hpp>

#include <boost/beast/websocket.hpp>

#include <iostream>
#include <random>
#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine {
namespace ps {

struct BattleParser {
	BattleParser(
		boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & websocket,
		std::string id_,
		std::string username,
		UsageStats const & usage_stats,
		Evaluate evaluate,
		Party party,
		std::string opponent,
		unsigned depth,
		std::mt19937 random_engine,
		Team team,
		TeamSize opponent_team_size
	):
		m_usage_stats(usage_stats),
		m_websocket(websocket),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_random_engine(random_engine),
		m_slot_memory(team.size()),
		m_battle(
			usage_stats,
			evaluate,
			party,
			std::move(opponent),
			depth,
			std::move(team),
			opponent_team_size,
			100_bi
		)
	{
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

	UsageStats const & m_usage_stats;
		
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket &> & m_websocket;
	std::string m_id;
	std::string m_username;
	std::mt19937 m_random_engine;

	SlotMemory m_slot_memory;

	Battle m_battle;
	bool m_completed = false;
	bool m_replacing_fainted = false;
};


struct BattleFactory {
	BattleFactory(
		std::string id_,
		std::string username,
		UsageStats const & usage_stats,
		Evaluate evaluate,
		unsigned depth,
		std::mt19937 random_engine,
		Team team
	):
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_usage_stats(usage_stats),
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
	UsageStats const & m_usage_stats;
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
