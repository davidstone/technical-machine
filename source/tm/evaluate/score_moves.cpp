// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.score_moves;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.move_probability;
import tm.evaluate.scored_move;

import tm.move.legal_selections;

import tm.environment;
import tm.generation;
import tm.team;

namespace technicalmachine {

export template<Generation generation>
auto score_moves(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth, MoveProbabilities const foe_moves) -> ScoredMoves;

#define EXTERN_INSTANTIATION(generation) \
	extern template auto score_moves(Team<generation> const & ai, LegalSelections const ai_selections, Team<generation> const & foe, LegalSelections const foe_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth, MoveProbabilities const foe_moves) -> ScoredMoves

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION);

} // namespace technicalmachine
