// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.battle_factory;

import tm.clients.ps.battle_interface;
import tm.clients.ps.battle_parser;

import tm.clients.write_team;

import tm.evaluate.all_evaluate;
import tm.evaluate.depth;

import tm.team_predictor.all_usage_stats;

import containers;
import tv;
import std_module;

namespace technicalmachine::ps {

export struct BattleFactory : BattleInterface {
	virtual auto make(
		AllUsageStats const & usage_stats,
		std::fileystem::path const & log_directory,
		tv::optional<WriteTeam> write_team
	) && -> BattleParser = 0;
};

export auto make_battle_factory(
	containers::string id,
	containers::string username,
	AllEvaluate evaluate,
	Depth depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleFactory>;

} // namespace technicalmachine::ps
