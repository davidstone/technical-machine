// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.move_probability;

import tm.move.legal_selections;
import tm.move.move_name;

import containers;

namespace technicalmachine {

export struct MoveProbability {
	MoveName name;
	double probability;
	friend auto operator==(MoveProbability, MoveProbability) -> bool = default;
};
export using MoveProbabilities = containers::static_vector<MoveProbability, maximum_possible_selections>;

} // namespace technicalmachine
