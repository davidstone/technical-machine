// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ps_usage_stats/rating.hpp>

#include <tm/generation_generic.hpp>
#include <tm/team.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine::ps_usage_stats {

struct BattleResult {
	struct Side {
		using ID = bounded::integer<0, bounded::builtin_max_value<std::uint64_t>>;
		GenerationGeneric<Team> team;
		ID id;
		bounded::optional<Rating> rating;
		friend auto operator==(Side const &, Side const &) -> bool = default;
	};
	enum class Winner {
		side1,
		side2,
		tie
	};
	Side side1;
	Side side2;
	Winner winner;

	friend auto operator==(BattleResult const &, BattleResult const &) -> bool = default;
};

} // namespace technicalmachine::ps_usage_stats
