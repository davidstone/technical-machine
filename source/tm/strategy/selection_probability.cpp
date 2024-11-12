// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.selection_probability;

import tm.move.legal_selections;
import tm.move.selection;

import bounded;
import containers;

namespace technicalmachine {

export struct SelectionProbability {
	Selection selection;
	double probability;
	friend constexpr auto operator==(SelectionProbability, SelectionProbability) -> bool = default;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::SelectionProbability> {
	static constexpr auto spare_representations = tombstone_traits<technicalmachine::Selection>::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::SelectionProbability {
		return technicalmachine::SelectionProbability(
			tombstone_traits<technicalmachine::Selection>::make(index),
			0.0
		);
	}
	static constexpr auto index(technicalmachine::SelectionProbability const & value) noexcept {
		return tombstone_traits<technicalmachine::Selection>::index(value.selection);
	}
};

namespace technicalmachine {

export using SelectionProbabilities = containers::static_vector<SelectionProbability, maximum_possible_selections>;

} // namespace technicalmachine
