// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_factory;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_parser;

import tm.clients.party;

import tm.evaluate.all_evaluate;
import tm.evaluate.analysis_logger;
import tm.evaluate.depth;

import tm.team_predictor.usage_stats;

import tm.generation;
import tm.team;

import containers;
import std_module;
import tv;

namespace technicalmachine::ps {

export struct BattleFactory : BattleInterface {
	virtual auto make() && -> BattleParser = 0;
};

export auto make_battle_factory(
	Generation generation,
	tv::variant<containers::string, Party> username,
	AllEvaluate evaluate,
	UsageStats const & usage_stats,
	Depth depth,
	AnalysisLogger analysis_logger
) -> std::unique_ptr<BattleFactory>;

} // namespace technicalmachine::ps
