// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.scored_action;

import tm.evaluate.extreme_element_value;

import tm.move.legal_selections;
import tm.move.move_name;

import containers;

namespace technicalmachine {

export struct ScoredAction {
	MoveName name;
	double score;
};
export using ScoredActions = containers::static_vector<ScoredAction, maximum_possible_selections>;

export constexpr auto max_score(ScoredActions const scored_actions) {
	return max_element_value(containers::transform(scored_actions, &ScoredAction::score));
}
} // namespace technicalmachine
