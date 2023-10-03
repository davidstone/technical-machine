// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.expectiminimax;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.extreme_element_value;
import tm.evaluate.scored_move;
import tm.evaluate.victory;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct ExpectiminimaxEvaluator {
	static constexpr auto operator()(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth, auto const function) -> ScoredMoves {
		auto scored_moves_shallower = depth.general > 1_bi ?
			ExpectiminimaxEvaluator()(
				ai,
				ai_selections,
				foe,
				foe_selections,
				environment,
				evaluate,
				one_level_deeper(depth),
				function
			) :
			ScoredMoves(containers::transform(ai_selections, [](MoveName const move) {
				return ScoredMove{move, 0.0};
			}));
		auto is_winner = [](ScoredMove const move) { return move.score >= victory<generation>; };
		if (auto const winner = containers::maybe_find_if(scored_moves_shallower, is_winner)) {
			return ScoredMoves({*winner});
		}
		return ScoredMoves(containers::transform(
			ai_selections,
			[&](MoveName const ai_move) {
				return ScoredMove{
					ai_move,
					min_element_value(
						containers::transform(foe_selections, [&](MoveName const foe_move) {
							return function(ai, ai_move, foe, foe_move, environment, depth);
						})
					)
				};
			}
		));
	}
};


export template<Generation generation>
auto expectiminimax(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) -> ScoredMoves {
	auto evaluator = Evaluator(evaluate, ExpectiminimaxEvaluator<generation>());
	return evaluator.select_type_of_move(ai, ai_selections, foe, foe_selections, environment, depth);
}

} // namespace technicalmachine
