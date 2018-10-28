// Handle Pokemon Showdown messages setting up a battle
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

#include "battle_parser.hpp"
#include "inmessage.hpp"
#include "json_parser.hpp"

#include <bounded/optional.hpp>
#include <containers/vector.hpp>

#include <boost/beast/websocket.hpp>

#include <iostream>
#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
namespace ps {

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
