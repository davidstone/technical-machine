// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/rating.hpp>

#include <tm/generation_generic.hpp>
#include <tm/team.hpp>

#include <bounded/integer.hpp>

#include <filesystem>

namespace technicalmachine::ps_usage_stats {

struct BattleResult {
	struct Side {
		GenerationGeneric<Team> team;
		bounded::optional<Rating> rating;
	};
	Side winner;
	Side loser;
};

auto parse_log(std::filesystem::path const & path) -> BattleResult;

} // namespace technicalmachine::ps_usage_stats
