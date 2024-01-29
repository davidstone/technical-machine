// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.expectiminimax;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.extreme_element_value;
import tm.evaluate.scored_selection;
import tm.evaluate.state;
import tm.evaluate.victory;
import tm.evaluate.win;

import tm.move.legal_selections;
import tm.move.move_name;
import tm.move.pass;
import tm.move.selection;

import tm.environment;
import tm.generation;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct ExpectiminimaxEvaluator {
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, auto const function) -> ScoredSelections {
		auto scored_selections_shallower = state.depth.general > 1_bi ?
			ExpectiminimaxEvaluator()(
				State<generation>(state.ai, state.foe, state.environment, one_level_deeper(state.depth)),
				ai_selections,
				foe_selections,
				evaluate,
				function
			) :
			ScoredSelections();
		auto is_winner = [](ScoredSelection const scored) { return scored.score >= victory<generation>; };
		if (auto const winner = containers::maybe_find_if(scored_selections_shallower, is_winner)) {
			return ScoredSelections({*winner});
		}
		return ScoredSelections(containers::transform(
			ai_selections,
			[&](Selection const ai_selection) {
				return ScoredSelection(
					ai_selection,
					min_element_value(
						containers::transform(foe_selections, [&](Selection const foe_selection) {
							return function(state, ai_selection, foe_selection);
						})
					)
				);
			}
		));
	}
};


export template<Generation generation>
auto expectiminimax(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate) -> ScoredSelections {
	if (auto const score = win(state.ai, state.foe)) {
		return ScoredSelections({ScoredSelection(pass, *score)});
	}
	auto evaluator = Evaluator(evaluate, ExpectiminimaxEvaluator<generation>());
	return evaluator.select_type_of_action(state, ai_selections, foe_selections);
}

} // namespace technicalmachine
