// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string_view>

export module tm.clients.ps.battles;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_factory;
import tm.clients.ps.battle_logger;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.battle_parser;
import tm.clients.ps.inmessage;
import tm.clients.ps.make_active;

import tm.clients.write_team;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;

import tm.generation;
import tm.team;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {
using namespace std::string_view_literals;

export struct Battles {
	explicit Battles(std::filesystem::path log_directory, tv::optional<WriteTeam> write_team):
		m_log_directory(std::move(log_directory)),
		m_write_team(std::move(write_team))
	{
		std::filesystem::create_directories(m_log_directory);
	}

	auto add_pending(
		containers::string id,
		containers::string username,
		AllEvaluate evaluate,
		Depth depth
	) -> void {
		containers::lazy_push_back(
			m_container,
			[&] {
				auto battle_logger = std::make_unique<BattleLogger>(m_log_directory / std::string_view(id), id);
				return Element{
					std::move(battle_logger),
					make_battle_factory(
						std::move(id),
						std::move(username),
						evaluate,
						depth
					)
				};
			}
		);
	}

	auto handle_message(AllUsageStats const & usage_stats, InMessage const message) -> tv::optional<BattleMessageResult> {
		auto matches_room = [&](auto const & element) {
			return element.battle->id() == message.room();
		};
		auto const it = containers::find_if(m_container, matches_room);
		if (it == containers::end(m_container)) {
			return tv::none;
		}
		it->logger->log(message);
		auto result = it->battle->handle_message(message);
		tv::visit(result, tv::overload(
			[](BattleContinues) {
			},
			[&](BattleResponseNeeded const &) {
			},
			[&](BattleStarted) {
				auto const battle_log_directory = m_log_directory / it->battle->id();
				make_active(usage_stats, AnalysisLogger(battle_log_directory), it->battle);
				auto const & battle = static_cast<BattleParser const &>(*it->battle);
				log_ai_team(battle.team(), battle_log_directory);
			},
			[&](BattleFinished) {
				containers::erase(m_container, it);
			}
		));
		return result;
	}

private:
	struct Element {
		// Must be `unique_ptr` because `ofstream` is not nothrow movable
		std::unique_ptr<BattleLogger> logger;
		std::unique_ptr<BattleInterface> battle;
	};

	auto log_ai_team(GenerationGeneric<Team> const & team, std::filesystem::path const & battle_log_directory) -> void {
		if (!m_write_team) {
			return;
		}
		auto const file_name = containers::concatenate<containers::string>(
			"team"sv,
			m_write_team->extension
		);
		m_write_team->function(
			team,
			battle_log_directory / std::string_view(file_name)
		);
	}

	std::filesystem::path m_log_directory;
	containers::vector<Element> m_container;
	tv::optional<WriteTeam> m_write_team;
};

} // namespace technicalmachine::ps
