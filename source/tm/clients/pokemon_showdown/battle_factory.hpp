// Handle Pokemon Showdown messages setting up a battle
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/clients/pokemon_showdown/battle_parser.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/evaluate/depth.hpp>
#include <tm/evaluate/evaluate.hpp>

#include <filesystem>
#include <memory>
#include <random>
#include <string>
#include <string_view>

namespace technicalmachine {

struct AllUsageStats;

namespace ps {

struct BattleFactory : BattleInterface {
	virtual auto make(AllUsageStats const & usage_stats) && -> BattleParser = 0;
};

auto make_battle_factory(
	std::filesystem::path const & base_log_directory,
	bool const log_foe_teams,
	containers::string id,
	containers::string username,
	AllEvaluate evaluate,
	DepthValues depth,
	std::mt19937 random_engine
) -> std::unique_ptr<BattleFactory>;

} // namespace ps
} // namespace technicalmachine
