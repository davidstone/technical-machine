// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.strategy.weighted_selection;

import tm.move.legal_selections;
import tm.move.selection;

import bounded;
import containers;
import std_module;

namespace technicalmachine {

export struct WeightedSelection {
	Selection selection;
	double weight;
	friend auto operator==(WeightedSelection, WeightedSelection) -> bool = default;
};

} // namespace technicalmachine

template<>
struct bounded::tombstone_traits<technicalmachine::WeightedSelection> {
	static constexpr auto spare_representations = tombstone_traits<technicalmachine::Selection>::spare_representations;

	static constexpr auto make(auto const index) noexcept -> technicalmachine::WeightedSelection {
		return technicalmachine::WeightedSelection(
			tombstone_traits<technicalmachine::Selection>::make(index),
			0.0
		);
	}
	static constexpr auto index(technicalmachine::WeightedSelection const & value) noexcept {
		return tombstone_traits<technicalmachine::Selection>::index(value.selection);
	}
};

namespace technicalmachine {

export using WeightedSelections = containers::static_vector<WeightedSelection, maximum_possible_selections>;

export auto pick_selection(WeightedSelections const selections, std::mt19937 & random_engine) -> Selection {
	auto const weights = containers::transform(selections, &WeightedSelection::weight);
	auto distribution = std::discrete_distribution(
		containers::legacy_iterator(containers::begin(weights)),
		containers::legacy_iterator(containers::end(weights))
	);
	return containers::at(selections, distribution(random_engine)).selection;
}

export struct BothWeightedSelections {
	WeightedSelections user;
	WeightedSelections predicted_other;
};

} // namespace technicalmachine