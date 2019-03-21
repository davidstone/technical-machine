// Handle Pokemon Showdown messages in a battle
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
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/move_state.hpp>
#include <tm/clients/pokemon_showdown/slot_memory.hpp>

#include <tm/clients/battle.hpp>

#include <tm/evaluate/evaluate.hpp>

#include <random>
#include <string>
#include <string_view>
#include <utility>

namespace technicalmachine {
namespace ps {

struct BattleParser {
	using SendMessageFunction = std::function<void(std::string_view)>;
	BattleParser(
		SendMessageFunction send_message,
		BattleLogger battle_logger,
		std::ofstream analysis_logger,
		std::string id_,
		std::string username,
		UsageStats const & usage_stats,
		Evaluate evaluate,
		Party party,
		unsigned const depth,
		std::mt19937 random_engine,
		Team ai,
		Team foe,
		bool log_foe_teams
	):
		m_usage_stats(usage_stats),
		m_send_message(std::move(send_message)),
		m_battle_logger(std::move(battle_logger)),
		m_analysis_logger(std::move(analysis_logger)),
		m_id(std::move(id_)),
		m_username(std::move(username)),
		m_random_engine(random_engine),
		m_slot_memory(ai.size()),
		m_evaluate(evaluate),
		m_battle(
			party,
			std::move(ai),
			std::move(foe)
		),
		m_depth(depth),
		m_log_foe_teams(log_foe_teams)
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
	void handle_damage(InMessage message);
	void maybe_use_previous_move();
	Moves determine_action();
	void send_move(Moves const move);
	void send_random_move();
	void send_message(std::string_view message);
	void handle_u_turn(Party const party);

	UsageStats const & m_usage_stats;
		
	SendMessageFunction m_send_message;
	BattleLogger m_battle_logger;
	std::ofstream m_analysis_logger;
	std::string m_id;
	std::string m_username;
	std::mt19937 m_random_engine;

	SlotMemory m_slot_memory;

	Evaluate m_evaluate;
	Battle m_battle;
	unsigned m_depth;
	MoveState m_move_state;
	bool m_log_foe_teams;
	bool m_completed = false;
	bool m_replacing_fainted = false;
};

constexpr auto make_party(std::string_view const player_id) {
	return 	Party(
		BOUNDED_CONDITIONAL(player_id == "p1", 0_bi,
		player_id == "p2" ? 1_bi :
		throw std::runtime_error("Invalid player id: " + std::string(player_id))
	));
}

// This adds additional string scanning, but I don't think the performance
// matters that much here
inline auto parse_details(std::string_view details) {
	auto parser = BufferView(details, ", ");
	auto const species = from_string<Species>(parser.next());
	auto const level_or_gender_str = parser.next();
	auto try_parse_gender = [](auto const str) {
		return
			BOUNDED_CONDITIONAL(str == "F", Gender::female,
			BOUNDED_CONDITIONAL(str == "M", Gender::male,
			BOUNDED_CONDITIONAL(str == "", Gender::genderless,
			bounded::none
		)));
	};
	auto const maybe_gender = try_parse_gender(level_or_gender_str);
	auto const level = Level(maybe_gender ? 100_bi : bounded::to_integer<Level::min, Level::max>(level_or_gender_str.substr(1)));
	auto parse_gender = [&]{
		if (maybe_gender) {
			return *maybe_gender;
		}
		auto const gender_str = parser.next();
		auto const gender = try_parse_gender(gender_str);
		if (!gender) {
			throw std::runtime_error("Invalid gender string " + std::string(gender_str));
		}
		return *gender;
	};
	auto const gender = parse_gender();
	struct result {
		Species species;
		Level level;
		Gender gender;
	};
	return result{species, level, gender};
}

struct ParsedSwitch {
	Party party;
	Species species;
	Level level;
	Gender gender;
	ParsedHP hp;
	Statuses status;
};
auto parse_switch(InMessage message) -> ParsedSwitch;

}	// namespace ps
}	// namespace technicalmachine
