// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.to_selection_probabilities;

import tm.evaluate.scored_selection;

import tm.strategy.selection_probability;

import tm.probability;

import containers;

namespace technicalmachine {

export constexpr auto to_selection_probabilities(ScoredSelections selections) -> SelectionProbabilities {
	auto const best = containers::max_element(
		selections,
		[](ScoredSelection const lhs, ScoredSelection const rhs) {
			return lhs.score > rhs.score;
		}
	);
	return SelectionProbabilities({
		SelectionProbability(best->selection, Probability(1.0))
	});
}

} // namespace technicalmachine
