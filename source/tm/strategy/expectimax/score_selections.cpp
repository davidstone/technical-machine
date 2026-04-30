// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.expectimax.score_selections;

import tm.evaluate.score;
import tm.evaluate.scored_selection;

import tm.move.legal_selections;
import tm.move.selection;

import tm.strategy.selection_probability;

import containers;
import std_module;

namespace technicalmachine {

auto parallel_sum(auto && range) {
	return std::reduce(
		std::execution::par_unseq,
		containers::legacy_iterator(containers::begin(range)),
		containers::legacy_iterator(containers::end(range)),
		Score(0.0)
	);
}

export auto score_selections(
	LegalSelections const ai_selections,
	SelectionProbabilities const foe_selections,
	auto const function
) -> ScoredSelections {
	return ScoredSelections(containers::transform(ai_selections, [&](Selection const ai_selection) {
		return ScoredSelection(
			ai_selection,
			parallel_sum(containers::transform(
				foe_selections,
				[&](SelectionProbability const predicted) {
					return predicted.probability * function(ai_selection, predicted.selection);
				}
			))
		);
	}));
}

} // namespace technicalmachine
