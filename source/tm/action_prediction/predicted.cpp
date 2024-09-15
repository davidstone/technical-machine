// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.action_prediction.predicted;

import tm.move.legal_selections;
import tm.move.selection;

import bounded;
import containers;

namespace technicalmachine {

export struct Predicted {
	Selection selection;
	double probability;
	friend auto operator==(Predicted, Predicted) -> bool = default;
};
export using AllPredicted = containers::static_vector<Predicted, maximum_possible_selections>;

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::Predicted> {
	static constexpr auto spare_representations = tombstone_traits<technicalmachine::Selection>::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::Predicted {
		return technicalmachine::Predicted(
			tombstone_traits<technicalmachine::Selection>::make(index),
			0.0
		);
	}
	static constexpr auto index(technicalmachine::Predicted const & value) noexcept {
		return tombstone_traits<technicalmachine::Selection>::index(value.selection);
	}
};