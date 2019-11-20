// Handle Pokemon Showdown messages setting up a battle
// Copyright (C) 2019 David Stone
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

#include <tm/clients/pokemon_showdown/battle_logger.hpp>
#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/json_parser.hpp>

#include <bounded/optional.hpp>
#include <containers/vector.hpp>

#include <random>
#include <string>
#include <utility>

namespace technicalmachine {

enum class Generation : std::uint8_t;

namespace ps {

struct BattleFactory {
	BattleFactory(
		std::filesystem::path const & base_log_directory,
		bool const log_foe_teams,
		std::string id_,
		std::string username,
		UsageStats const & usage_stats,
		Evaluate evaluate,
		unsigned depth,
		std::mt19937 random_engine,
		Team team
	):
		m_id(std::move(id_)),
		m_log_directory(base_log_directory / m_id),
		m_battle_logger(m_log_directory, m_id),
		m_username(std::move(username)),
		m_usage_stats(usage_stats),
		m_evaluate(evaluate),
		m_depth(depth),
		m_random_engine(random_engine),
		m_generation(parse_generation(m_id)),
		m_team(std::move(team)),
		m_log_foe_teams(log_foe_teams)
	{
	}
	
	std::string_view id() const {
		return m_id;
	}
	
	void handle_message(InMessage message);
	
	bool completed() const {
		// TODO: Handle NvN battles
		return m_ai_switched_in and m_foe_starter;
	}
	BattleParser make(BattleParser::SendMessageFunction send_message) &&;

private:
	enum class Clause { };

	static auto parse_generation(std::string const & id) -> Generation;

	JSONParser m_parse_json;

	std::string m_id;
	std::filesystem::path m_log_directory;
	BattleLogger m_battle_logger;
	std::string m_username;
	UsageStats const & m_usage_stats;
	Evaluate m_evaluate;
	unsigned m_depth;
	std::mt19937 m_random_engine;
	Generation m_generation;
	Team m_team;
	containers::vector<Clause> m_rules;
	bounded::optional<Party> m_party;
	bounded::optional<std::string> m_type;	// singles, doubles, triples
	bounded::optional<std::string> m_tier;
	bounded::optional<TeamSize> m_foe_team_size;
	bounded::optional<ParsedSwitch> m_foe_starter;
	bool m_ai_switched_in = false;
	bool m_log_foe_teams;
};

}	// namespace ps
}	// namespace technicalmachine
