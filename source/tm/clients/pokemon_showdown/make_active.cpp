// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.make_active;

import tm.clients.ps.battle_factory;
import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_parser;

import tm.evaluate.analysis_logger;

import tm.team_predictor.all_usage_stats;

import tv;
import std_module;

namespace technicalmachine::ps {

export auto make_active(
	AllUsageStats const & usage_stats,
	AnalysisLogger analysis_logger,
	std::unique_ptr<BattleInterface> & battle
) -> void {
	auto & battle_factory = static_cast<BattleFactory &>(*battle);
	battle = std::unique_ptr<BattleInterface>(new BattleParser(
		std::move(battle_factory).make(
			usage_stats,
			std::move(analysis_logger)
		)
	));
}

} // namespace technicalmachine::ps
