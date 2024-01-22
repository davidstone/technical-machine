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
			ScoredActions(containers::transform(ai_selections, [](MoveName const move) {
				return ScoredAction(move, 0.0);
			}));
		auto is_winner = [](ScoredAction const sa) { return sa.score >= victory<generation>; };
		if (auto const winner = containers::maybe_find_if(scored_actions_shallower, is_winner)) {
			return ScoredActions({*winner});
		}
		return ScoredActions(containers::transform(
			ai_selections,
			[&](MoveName const ai_move) {
				return ScoredAction(
					ai_move,
					min_element_value(
						containers::transform(foe_selections, [&](MoveName const foe_move) {
							return function(state, ai_move, foe_move);
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
	return evaluator.select_type_of_move(state, ai_selections, foe_selections);
}

} // namespace technicalmachine
