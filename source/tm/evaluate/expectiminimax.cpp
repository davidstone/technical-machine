// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.expectiminimax;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.extreme_element_value;
import tm.evaluate.scored_action;
import tm.evaluate.state;
import tm.evaluate.victory;
import tm.evaluate.win;

import tm.move.action;
import tm.move.legal_selections;
import tm.move.move_name;

import tm.environment;
import tm.generation;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct ExpectiminimaxEvaluator {
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, auto const function) -> ScoredActions {
		auto scored_actions_shallower = state.depth.general > 1_bi ?
			ExpectiminimaxEvaluator()(
				State<generation>(state.ai, state.foe, state.environment, one_level_deeper(state.depth)),
				ai_selections,
				foe_selections,
				evaluate,
				function
			) :
			ScoredActions();
		auto is_winner = [](ScoredAction const sa) { return sa.score >= victory<generation>; };
		if (auto const winner = containers::maybe_find_if(scored_actions_shallower, is_winner)) {
			return ScoredActions({*winner});
		}
		return ScoredActions(containers::transform(
			ai_selections,
			[&](Action const ai_action) {
				return ScoredAction(
					ai_action,
					min_element_value(
						containers::transform(foe_selections, [&](Action const foe_action) {
							return function(state, ai_action, foe_action);
						})
					)
				);
			}
		));
	}
};


export template<Generation generation>
auto expectiminimax(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate) -> ScoredActions {
	if (auto const score = win(state.ai, state.foe)) {
		return ScoredActions({ScoredAction(MoveName::Pass, *score)});
	}
	auto evaluator = Evaluator(evaluate, ExpectiminimaxEvaluator<generation>());
	return evaluator.select_type_of_action(state, ai_selections, foe_selections);
}

} // namespace technicalmachine
