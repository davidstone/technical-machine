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
import tm.clients.ps.parse_generation_from_format;

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
		AllUsageStats const & all_usage_stats,
		Depth depth
	) -> void {
		auto const generation = parse_generation_from_format(id, "battle-gen");
		auto const battle_log_directory = m_log_directory / std::string_view(id);
		auto battle_logger = std::make_unique<BattleLogger>(battle_log_directory, id);
		auto battle_factory = make_battle_factory(
			generation,
			std::move(username),
			evaluate,
			all_usage_stats[generation],
			depth,
			AnalysisLogger(battle_log_directory)
		);
		containers::emplace_back(
			m_container,
			std::move(id),
			std::move(battle_logger),
			std::move(battle_factory)
		);
	}

	auto handle_message(InMessage const message) -> tv::optional<BattleMessageResult> {
		auto matches_room = [&](auto const & element) {
			return element.id == message.room();
		};
		auto const it = containers::find_if(m_container, matches_room);
		if (it == containers::end(m_container)) {
			return tv::none;
		}
		it->logger->log(message);
		auto result = it->battle->handle_message(message);
		tv::visit(result, tv::overload(
			[](auto) {
			},
			[&](BattleStarted) {
				auto const battle_log_directory = m_log_directory / std::string_view(it->id);
				auto & battle_factory = static_cast<BattleFactory &>(*it->battle);
				it->battle = std::unique_ptr<BattleInterface>(new BattleParser(
					std::move(battle_factory).make()
				));
				log_ai_team(static_cast<BattleParser const &>(*it->battle).team(), battle_log_directory);
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
