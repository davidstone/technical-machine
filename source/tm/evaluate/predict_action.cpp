// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.evaluate.predict_action;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.action_probability;

import tm.move.legal_selections;

import tm.environment;
import tm.generation;
import tm.team;

namespace technicalmachine {

export template<Generation generation>
auto predict_action(Team<generation> const & team, LegalSelections const selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> evaluate, Depth const depth) -> ActionProbabilities;

#define EXTERN_INSTANTIATION(generation) \
	extern template auto predict_action(Team<generation> const & team, LegalSelections const selections, Team<generation> const & other, LegalSelections const other_selections, Environment const environment, Evaluate<generation> const evaluate, Depth const depth) -> ActionProbabilities

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION);

} // namespace technicalmachine
