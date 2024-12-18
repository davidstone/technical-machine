// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.selection_probability;

import tm.move.legal_selections;
import tm.move.selection;

import tm.probability;

import bounded;
import containers;

namespace technicalmachine {

export struct SelectionProbability {
	Selection selection;
	Probability probability;
	friend constexpr auto operator==(SelectionProbability, SelectionProbability) -> bool = default;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone<technicalmachine::SelectionProbability> {
	static constexpr auto make(auto const index) noexcept -> technicalmachine::SelectionProbability {
		return technicalmachine::SelectionProbability(
			tombstone_traits<technicalmachine::Selection>::make(index),
			technicalmachine::Probability(0.0)
		);
	}
	static constexpr auto index(technicalmachine::SelectionProbability const & value) noexcept {
		return tombstone_traits<technicalmachine::Selection>::index(value.selection);
	}
};

namespace technicalmachine {

export using SelectionProbabilities = containers::static_vector<SelectionProbability, maximum_possible_selections>;

} // namespace technicalmachine
