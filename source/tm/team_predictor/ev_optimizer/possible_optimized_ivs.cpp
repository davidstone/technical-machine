// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.ev_optimizer.possible_optimized_ivs;

import tm.stat.iv;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto max_possible_optimized_ivs = 2_bi;
export using PossibleOptimizedIVs = containers::static_vector<IV, max_possible_optimized_ivs>;

export constexpr auto possible_optimized_ivs(containers::range auto const ivs) -> PossibleOptimizedIVs {
	return PossibleOptimizedIVs(containers::take(containers::reversed(ivs), max_possible_optimized_ivs));
};

} // namespace technicalmachine
