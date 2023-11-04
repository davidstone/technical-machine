// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.evaluate.score_moves;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.move_probability;
import tm.evaluate.predict_action;
import tm.evaluate.scored_move;
import tm.evaluate.state;
import tm.evaluate.team_is_empty;

import tm.move.legal_selections;
import tm.move.move_name;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct ScoreMovesEvaluator {
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections, Evaluate<generation>, MoveProbabilities const foe_moves, auto const function) -> ScoredMoves {
		return ScoredMoves(containers::transform(ai_selections, [&](MoveName const ai_move) {
			return ScoredMove{
				ai_move,
				containers::sum(containers::transform(
					foe_moves,
					[&](MoveProbability const foe_move) {
						return foe_move.probability * function(state, ai_move, foe_move.name);
					}
				))
			};
		}));
	}
	static constexpr auto operator()(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, auto const function) -> ScoredMoves {
		return ScoreMovesEvaluator()(
			state,
			ai_selections,
			foe_selections,
			evaluate,
			predict_action(
				state.foe,
				foe_selections,
				state.ai,
				ai_selections,
				state.environment,
				evaluate,
				Depth(1_bi, 1_bi)
			),
			std::move(function)
		);
	}
};

template<Generation generation>
auto score_moves(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, MoveProbabilities const foe_moves) -> ScoredMoves {
	if (team_is_empty(state.ai) or team_is_empty(state.foe)) {
		throw std::runtime_error("Tried to evaluate a position with an empty team");
	}
	auto evaluator = Evaluator(evaluate, ScoreMovesEvaluator<generation>());
	auto const moves = evaluator.select_type_of_move(
		state,
		ai_selections,
		foe_selections,
		foe_moves
	);
	if (containers::maybe_find_if(moves, [](ScoredMove const move) { return move.name == MoveName::Pass; })) {
		throw std::runtime_error("Should never evaluate a position in which it is legal to use Pass.");
	}
	return moves;
}

#define INSTANTIATE(generation) \
	template auto score_moves(State<generation> const & state, LegalSelections const ai_selections, LegalSelections const foe_selections, Evaluate<generation> const evaluate, MoveProbabilities const foe_moves) -> ScoredMoves

TM_FOR_EACH_GENERATION(INSTANTIATE);

} // namespace technicalmachine