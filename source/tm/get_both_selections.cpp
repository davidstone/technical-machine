// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.get_both_selections;

import tm.evaluate.predicted;
import tm.evaluate.predict_random_selection;

import tm.move.legal_selections;

import tm.environment;
import tm.generation;
import tm.get_legal_selections;
import tm.team;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

struct BothActions {
	AllPredicted predicted;
	LegalSelections other;
};

export template<Generation generation>
auto get_both_selections(Team<generation> const & team, Team<generation> const & other, Environment const environment) -> BothActions {
	auto const team_selections = get_legal_selections(team, other, environment);
	auto const other_selections = get_legal_selections(other, team, environment);
	auto predicted = predict_random_selection(team_selections, 0.164);
	return BothActions(predicted, other_selections);
}

} // namespace technicalmachine
