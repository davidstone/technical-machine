// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.score_selections;

import tm.evaluate.evaluate;
import tm.evaluate.predicted;
import tm.evaluate.scored_selection;
import tm.evaluate.state;

import tm.move.legal_selections;

import tm.generation;

namespace technicalmachine {

export template<Generation generation>
auto score_selections(State<generation> const &, LegalSelections ai_selections, AllPredicted foe_selections, Evaluate<generation>) -> ScoredSelections;

#define EXTERN_INSTANTIATION(generation) \
	extern template auto score_selections(State<generation> const &, LegalSelections ai_selections, AllPredicted foe_selections, Evaluate<generation>) -> ScoredSelections

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION);

} // namespace technicalmachine
