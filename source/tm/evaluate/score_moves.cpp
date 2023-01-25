// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.score_moves;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.evaluator;
import tm.evaluate.predict_action;
import tm.evaluate.scored_move;
import tm.evaluate.team_is_empty;

import tm.move.max_moves_per_pokemon;
import tm.move.move_name;

import tm.block;
import tm.generation;
import tm.team;
import tm.weather;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
struct ScoreMovesEvaluator {
	static constexpr auto operator()(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections, Weather const weather, Evaluate<generation>, Depth const depth, MoveProbabilities const foe_moves, auto const function) -> ScoredMoves {
		return ScoredMoves(containers::transform(ai_selections, [&](MoveName const ai_move) {
			return ScoredMove{
				ai_move,
				containers::sum(containers::transform(
					foe_moves,
					[&](MoveProbability const foe_move) {
						return foe_move.probability * function(ai, ai_move, foe, foe_move.name, weather, depth);
					}
				))
			};
		}));
	}
	static constexpr auto operator()(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Weather const weather, Evaluate<generation> const evaluate, Depth const depth, auto const function) -> ScoredMoves {
		return ScoreMovesEvaluator()(
			ai,
			ai_selections,
			foe,
			foe_selections,
			weather,
			evaluate,
			depth,
			predict_action(foe, foe_selections, ai, ai_selections, weather, evaluate),
			std::move(function)
		);
	}
};

export template<Generation generation>
auto score_moves(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Weather const weather, Evaluate<generation> const evaluate, Depth const depth, MoveProbabilities const foe_moves) -> ScoredMoves {
	if (team_is_empty(ai) or team_is_empty(foe)) {
		throw std::runtime_error("Tried to evaluate a position with an empty team");
	}
	auto evaluator = Evaluator(evaluate, ScoreMovesEvaluator<generation>());
	auto const moves = evaluator.select_type_of_move(ai, ai_selections, foe, foe_selections, weather, depth, foe_moves);
	if (containers::maybe_find_if(moves, [](ScoredMove const move) { return move.name == MoveName::Pass; })) {
		throw std::runtime_error("Should never evaluate a position in which it is legal to use Pass.");
	}
	return moves;
}

#define TECHNICALMACHINE_EXPLICIT_INSTANTIATION(generation) \
	template auto score_moves(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Weather const weather, Evaluate<generation> const evaluate, Depth const depth, MoveProbabilities const foe_moves) -> ScoredMoves

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXPLICIT_INSTANTIATION);

} // namespace technicalmachine
