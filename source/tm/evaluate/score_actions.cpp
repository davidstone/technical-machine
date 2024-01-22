// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.score_actions;

import tm.evaluate.evaluate;
import tm.evaluate.action_probability;
import tm.evaluate.scored_move;
import tm.evaluate.state;

import tm.move.legal_selections;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
auto score_actions(State<generation> const &, LegalSelections ai_selections, ActionProbabilities foe_selections, Evaluate<generation>) -> ScoredMoves;

#define EXTERN_INSTANTIATION(generation) \
	extern template auto score_actions(State<generation> const &, LegalSelections ai_selections, ActionProbabilities foe_moves, Evaluate<generation>) -> ScoredMoves

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION);

} // namespace technicalmachine
