// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.evaluate.score_selections;

import tm.action_prediction.predict_random_selection;
import tm.action_prediction.predicted;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.scored_selection;
import tm.evaluate.state;
import tm.evaluate.win;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;

import tm.environment;
import tm.generation;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

auto parallel_sum(auto && range) {
	return std::reduce(
		std::execution::par_unseq,
		containers::legacy_iterator(containers::begin(range)),
		containers::legacy_iterator(containers::end(range)),
		0.0
	);
}

template<Generation generation>
struct ScoreMovesEvaluator {
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, AllPredicted const foe_selections, Evaluate<generation>, auto const function) -> ScoredSelections {
		return ScoredSelections(containers::transform(ai_selections, [&](Selection const ai_selection) {
			return ScoredSelection(
				ai_selection,
				parallel_sum(containers::transform(
					foe_selections,
					[&](Predicted const predicted) {
						return predicted.probability * function(state, ai_selection, predicted.selection);
					}
				))
			);
		}));
	}
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, auto const function) -> ScoredSelections {
		return ScoreMovesEvaluator()(
			state,
			ai_selections,
			predict_random_selection(foe_selections, 0.164),
			evaluate,
			std::move(function)
		);
	}
};

template<Generation generation>
auto score_selections(State<generation> const & state, LegalSelections const ai_selections, AllPredicted const foe_selections, Evaluate<generation> const evaluate) -> ScoredSelections {
	if (auto const score = win(state.ai, state.foe)) {
		return ScoredSelections({ScoredSelection(pass, *score)});
	}
	auto evaluator = Evaluator(evaluate, ScoreMovesEvaluator<generation>());
	return evaluator.select_type_of_action(
		state,
		ai_selections,
		foe_selections
	);
}

#define INSTANTIATE(generation) \
	template auto score_selections(State<generation> const &, LegalSelections ai_selections, AllPredicted foe_selections, Evaluate<generation>) -> ScoredSelections

TM_FOR_EACH_GENERATION(INSTANTIATE);

} // namespace technicalmachine
