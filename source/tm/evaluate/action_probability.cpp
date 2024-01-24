// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.action_probability;

import tm.move.action;
import tm.move.legal_selections;

import containers;

namespace technicalmachine {

export struct ActionProbability {
	Action action;
	double probability;
	friend auto operator==(ActionProbability, ActionProbability) -> bool = default;
};
export using ActionProbabilities = containers::static_vector<ActionProbability, maximum_possible_selections>;

} // namespace technicalmachine
