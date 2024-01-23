// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.evaluate.score_actions;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.action_probability;
import tm.evaluate.predict_action;
import tm.evaluate.scored_action;
import tm.evaluate.state;
import tm.evaluate.win;

import tm.move.legal_selections;
import tm.move.move_name;

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
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, ActionProbabilities const foe_actions, Evaluate<generation>, auto const function) -> ScoredActions {
		return ScoredActions(containers::transform(ai_selections, [&](MoveName const ai_move) {
			return ScoredAction(
				ai_move,
				parallel_sum(containers::transform(
					foe_actions,
					[&](ActionProbability const foe_action) {
						return foe_action.probability * function(state, ai_move, foe_action.name);
					}
				))
			);
		}));
	}
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, auto const function) -> ScoredActions {
		return ScoreMovesEvaluator()(
			state,
			ai_selections,
			predict_action(
				state.foe,
				foe_selections,
				state.ai,
				ai_selections,
				state.environment,
				evaluate,
				Depth(1_bi, 1_bi)
			),
			evaluate,
			std::move(function)
		);
	}
};

template<Generation generation>
auto score_actions(State<generation> const & state, LegalSelections const ai_selections, ActionProbabilities const foe_actions, Evaluate<generation> const evaluate) -> ScoredActions {
	if (auto const score = win(state.ai, state.foe)) {
		return ScoredActions({ScoredAction(MoveName::Pass, *score)});
	}
	auto evaluator = Evaluator(evaluate, ScoreMovesEvaluator<generation>());
	return evaluator.select_type_of_move(
		state,
		ai_selections,
		foe_actions
	);
}

#define INSTANTIATE(generation) \
	template auto score_actions(State<generation> const &, LegalSelections ai_selections, ActionProbabilities foe_actions, Evaluate<generation>) -> ScoredActions

TM_FOR_EACH_GENERATION(INSTANTIATE);

} // namespace technicalmachine
