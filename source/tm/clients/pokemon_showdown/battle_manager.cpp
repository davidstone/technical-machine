// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_manager;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_factory;
import tm.clients.ps.battle_message_result;
import tm.clients.ps.battle_parser;
import tm.clients.ps.in_message;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.move.move_name;

import tm.team_predictor.usage_stats;

import tm.generation;
import tm.generation_generic;
import tm.team;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {

export struct BattleManager {
	explicit BattleManager(
		Generation const generation,
		tv::variant<containers::string, Party> user,
		AllEvaluate evaluate,
		UsageStats const & usage_stats,
		Depth depth,
		AnalysisLogger logger
	):
		m_battle(make_battle_factory(
			generation,
			std::move(user),
			evaluate,
			usage_stats,
			depth,
			std::move(logger)
		))
	{
	}
	explicit BattleManager(
		GenerationGeneric<KnownTeam> team,
		tv::variant<containers::string, Party> user,
		AllEvaluate evaluate,
		UsageStats const & usage_stats,
		Depth depth,
		AnalysisLogger logger
	):
		m_battle(make_battle_factory(
			std::move(team),
			std::move(user),
			evaluate,
			usage_stats,
			depth,
			std::move(logger)
		))
	{
	}

	auto handle_message(InMessage const message) -> BattleMessageResult {
		auto result = m_battle->handle_message(message);
		tv::visit(result, tv::overload(
			[](auto) {
			},
			[&](BattleStarted) {
				auto & battle_factory = static_cast<BattleFactory &>(*m_battle);
				m_battle = std::unique_ptr<BattleInterface>(new BattleParser(
					std::move(battle_factory).make()
				));
			},
			[&](BattleFinished) {
			}
		));
		return result;
	}

	// Requires that the battle has begun
	auto team() const -> GenerationGeneric<Team> {
		return static_cast<BattleParser const &>(*m_battle).team();
	}

private:
	std::unique_ptr<BattleInterface> m_battle;
};

} // namespace technicalmachine::ps
