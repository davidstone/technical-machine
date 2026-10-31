export module tm.strategy.expectimax;

import tm.evaluate.depth;

import tm.strategy.strategy;

namespace technicalmachine {

export auto make_expectimax(
	Depth const depth,
	Strategy foe_strategy
) -> Strategy;

} // namespace technicalmachine
