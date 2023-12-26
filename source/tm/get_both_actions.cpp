// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.get_both_actions;

import tm.evaluate.depth;
import tm.evaluate.evaluate;
import tm.evaluate.move_probability;
import tm.evaluate.predict_action;

import tm.move.legal_selections;

import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.team;
import tm.team_is_empty;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

struct BothActions {
	MoveProbabilities predicted;
	LegalSelections other;
};

export template<Generation generation>
auto get_both_actions(Team<generation> const & team, Team<generation> const & other, Environment const environment, Evaluate<generation> const evaluate) -> BothActions {
	if (team_is_empty(team) or team_is_empty(other)) {
		throw std::runtime_error("Tried to determine an action with an empty team.");
	}
	auto const team_selections = get_legal_selections(team, other, environment);
	auto const other_selections = get_legal_selections(other, team, environment);
	auto predicted = predict_action(
		team,
		team_selections,
		other,
		other_selections,
		environment,
		evaluate,
		Depth(1_bi, 1_bi)
	);
	return BothActions(predicted, other_selections);
}

#define INSTANTIATE(generation) \
	extern template auto get_both_actions(Team<generation> const & team, Team<generation> const & other, Environment const environment, Evaluate<generation> const evaluate) -> BothActions; \
	template auto get_both_actions(Team<generation> const & team, Team<generation> const & other, Environment const environment, Evaluate<generation> const evaluate) -> BothActions

TM_FOR_EACH_GENERATION(INSTANTIATE);


} // namespace technicalmachine
