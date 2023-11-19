// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.ps_usage_stats.battle_result;

import tm.ps_usage_stats.rating;

import tm.generation_generic;
import tm.team;

import bounded;
import tv;
import std_module;

namespace technicalmachine::ps_usage_stats {

export struct BattleResult {
	struct Side {
		using ID = bounded::integer<0, bounded::builtin_max_value<std::uint64_t>>;
		GenerationGeneric<Team> team;
		ID id;
		tv::optional<Rating> rating;
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
