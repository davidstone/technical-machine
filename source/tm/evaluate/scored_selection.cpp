// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.scored_selection;

import tm.evaluate.extreme_element_value;
import tm.evaluate.score;

import tm.move.legal_selections;
import tm.move.selection;

import containers;

namespace technicalmachine {

export struct ScoredSelection {
	Selection selection;
	Score score;
};
export using ScoredSelections = containers::static_vector<ScoredSelection, maximum_possible_selections>;

export constexpr auto max_score(ScoredSelections const scored_actions) {
	return max_element_value(containers::transform(scored_actions, &ScoredSelection::score));
}
} // namespace technicalmachine
