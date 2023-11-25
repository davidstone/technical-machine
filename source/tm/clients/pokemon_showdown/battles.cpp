// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.battles;

import tm.clients.ps.battle_logger;
import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.room_message;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.team;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export struct Battles {
	explicit Battles(std::filesystem::path log_directory):
		m_log_directory(std::move(log_directory))
	{
		std::filesystem::create_directories(m_log_directory);
	}

	auto add_pending(
		containers::string id,
		tv::variant<containers::string, Party> user,
		AllEvaluate evaluate,
		AllUsageStats const & all_usage_stats,
		Depth depth
	) -> void {
		auto const generation = parse_generation_from_format(id, "battle-gen");
		auto const battle_log_directory = m_log_directory / std::string_view(id);
		auto battle_logger = std::make_unique<BattleLogger>(battle_log_directory, id);
		auto battle_manager = BattleManager(
			generation,
			std::move(user),
			evaluate,
			all_usage_stats[generation],
			depth,
			AnalysisLogger(battle_log_directory)
		);
		containers::emplace_back(
			m_container,
			std::move(id),
			std::move(battle_logger),
			std::move(battle_manager)
		);
	}

	auto handle_message(RoomMessage const room_message) -> tv::optional<BattleMessageResult> {
		auto matches_room = [&](auto const & element) {
			return element.id == room_message.room;
		};
		auto const it = containers::find_if(m_container, matches_room);
		if (it == containers::end(m_container)) {
			return tv::none;
		}
		it->logger->log(room_message.message);
		auto result = it->battle.handle_message(room_message.message);
		tv::visit(result, tv::overload(
			[](auto) {
			},
			[&](BattleFinished) {
				containers::erase(m_container, it);
			}
		));
		return result;
	}

private:
	struct Element {
		containers::string id;
		// Must be `unique_ptr` because `ofstream` is not nothrow movable
		std::unique_ptr<BattleLogger> logger;
		BattleManager battle;
	};

	std::filesystem::path m_log_directory;
	containers::vector<Element> m_container;
};

} // namespace technicalmachine::ps
