// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax;

import tm.evaluate.depth;

import tm.strategy.strategy;

namespace technicalmachine {

export auto make_expectimax(
	Depth const depth,
	Strategy foe_strategy
) -> Strategy;

} // namespace technicalmachine
