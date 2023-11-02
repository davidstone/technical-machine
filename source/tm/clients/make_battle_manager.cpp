// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.make_battle_manager;

import tm.clients.battle_manager;
import tm.clients.make_battle_manager_inputs;

import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.team_predictor.usage_stats;

import tm.generation;

import tv;
import std_module;

namespace technicalmachine {

// `usage_stats` must remain valid for the lifetime of the return value
export auto make_battle_manager(
	AnalysisLogger analysis_logger,
	UsageStats const & usage_stats,
	GenerationGeneric<BattleManagerInputs> generic_inputs,
	Depth const depth
) -> std::unique_ptr<BattleManager>;

} // namespace technicalmachine
