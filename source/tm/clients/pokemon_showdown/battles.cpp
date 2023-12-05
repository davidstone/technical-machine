// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.battles;

import tm.clients.ps.battle_manager;
import tm.clients.ps.battle_message;
import tm.clients.ps.battle_message_handler;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.parse_generation_from_format;
import tm.clients.ps.room;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;
import tm.team_predictor.usage_stats;

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
	}

	auto handle_message(
		Room const room,
		BattleMessage const & generic_message,
		std::string_view const user,
		AllEvaluate evaluate,
		AllUsageStats const & all_usage_stats,
		Depth depth
	) -> BattleMessageResult {
		auto const it = m_container.find(room);
		auto const result = tv::visit(generic_message, tv::overload(
			[&](CreateBattle) -> BattleMessageResult {
				if (it != containers::end(m_container)) {
					throw std::runtime_error("Tried to create an existing battle");
				}
				create_battle(room);
				return BattleContinues();
			},
			[&](auto const & message) -> BattleMessageResult {
				if (it == containers::end(m_container)) {
					return BattleAlreadyFinished();
				}
				return it->mapped.handle_message(
					message,
					user,
					room,
					m_log_directory,
					evaluate,
					all_usage_stats,
					depth
				);
			}
		));
		tv::visit(result, tv::overload(
			[](auto) {
			},
			[&](BattleFinished) {
				m_container.erase(it);
			}
		));
		return result;
	}

private:
	auto create_battle(Room const room) -> void {
		m_container.lazy_insert(
			containers::string(room),
			[&] {
				return BattleManager(parse_generation_from_format(room, "battle-gen"sv));
			}
		);
	}

	std::filesystem::path m_log_directory;
	containers::linear_map<containers::string, BattleManager> m_container;
};

} // namespace technicalmachine::ps
