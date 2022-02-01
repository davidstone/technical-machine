// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/estimate.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/at.hpp>
#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>

#include <algorithm>
#include <random>

namespace technicalmachine {

Estimate::Estimate(UsageStats const & usage_stats) {
	for (auto const species : containers::enum_range<Species>()) {
		m_estimate[bounded::integer(species)] = usage_stats.get(species).weight;
	}
}

auto Estimate::update(UsageStats const & usage_stats, Species const species) -> void {
	auto const & per_species = usage_stats.get(species);
	for (auto const predicted : containers::enum_range<Species>()) {
		auto const index = bounded::integer(predicted);
		m_estimate[index] *= per_species.teammates[index];
	}
}

auto Estimate::most_likely() const -> Species {
	auto const it = std::max_element(
		containers::legacy_iterator(containers::begin(m_estimate)),
		containers::legacy_iterator(containers::end(m_estimate))
	).base();
	// Estimate is indexed by Species ID. The Species ID is therefore the
	// distance between the found element and the beginning.
	return static_cast<Species>(it - containers::begin(m_estimate));
}

auto Estimate::random(std::mt19937 & random_engine) const -> Species {
	auto const total = containers::sum(m_estimate);
	auto distribution = std::uniform_real_distribution<float>(0.0F, total);
	auto usage_threshold = distribution(random_engine);
	for (auto const species : containers::enum_range<Species>()) {
		usage_threshold -= containers::at(m_estimate, species);
		if (usage_threshold <= 0.0F) {
			return species;
		}
	}
	// In case rounding leads to a minor error
	return numeric_traits::max_value<Species>;
}

}	// namespace technicalmachine
