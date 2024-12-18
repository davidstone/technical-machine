// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.weighted_selection;

import tm.move.legal_selections;
import tm.move.selection;

import tm.weight;

import bounded;
import containers;

namespace technicalmachine {

export struct WeightedSelection {
	Selection selection;
	Weight<double> weight;
	friend auto operator==(WeightedSelection, WeightedSelection) -> bool = default;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::WeightedSelection> {
	static constexpr auto make(auto const index) noexcept -> technicalmachine::WeightedSelection {
		return technicalmachine::WeightedSelection(
			tombstone_traits<technicalmachine::Selection>::make(index),
			technicalmachine::Weight(0.0)
		);
	}
	static constexpr auto index(technicalmachine::WeightedSelection const & value) noexcept {
		return tombstone_traits<technicalmachine::Selection>::index(value.selection);
	}
};

namespace technicalmachine {

export using WeightedSelections = containers::static_vector<WeightedSelection, maximum_possible_selections>;

export struct BothWeightedSelections {
	WeightedSelections user;
	WeightedSelections predicted_other;
};

} // namespace technicalmachine