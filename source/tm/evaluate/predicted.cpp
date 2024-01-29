// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.evaluate.predicted;

import tm.move.legal_selections;
import tm.move.selection;

import containers;

namespace technicalmachine {

export struct Predicted {
	Selection selection;
	double probability;
	friend auto operator==(Predicted, Predicted) -> bool = default;
};
export using AllPredicted = containers::static_vector<Predicted, maximum_possible_selections>;

} // namespace technicalmachine
