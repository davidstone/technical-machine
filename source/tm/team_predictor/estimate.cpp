// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/team_predictor/estimate.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/begin_end.hpp>
#include <containers/integer_range.hpp>

#include <algorithm>
#include <cstddef>
#include <random>

namespace technicalmachine {

Estimate::Estimate(UsageStats const & usage_stats, LeadStats const lead_stats) {
	auto const & lead = lead_stats.get(usage_stats);
	auto const total = usage_stats.total_usage();
	for (auto const species : containers::enum_range<Species>()) {
		m_estimate[bounded::integer(species)] = lead[bounded::integer(species)] * usage_stats.get(species).usage / static_cast<float>(total);
	}
}

void Estimate::update(UsageStats const & usage_stats, Species const seen) {
	for (auto const predicted : containers::enum_range<Species>()) {
		m_estimate[bounded::integer(predicted)] *= usage_stats.get(seen).teammates[bounded::integer(predicted)];
	}
}

Species Estimate::most_likely() const {
	auto const it = std::max_element(containers::begin(m_estimate), containers::end(m_estimate));
	// Estimate is indexed by Species ID. The Species ID is therefore the
	// distance between the found element and the beginning.
	return static_cast<Species>(it - containers::begin(m_estimate));
}

Species Estimate::random(std::mt19937 & random_engine) const {
	auto const total = containers::sum(m_estimate);
	auto distribution = std::uniform_real_distribution<float>(0.0F, total);
	auto usage_threshold = distribution(random_engine);
	auto const it = containers::find_if(m_estimate, [=](auto const value) mutable {
		usage_threshold -= value;
		return usage_threshold <= 0.0F;
	});
	BOUNDED_ASSERT(it != containers::end(m_estimate));
	return static_cast<Species>(it - containers::begin(m_estimate));
}

}	// namespace technicalmachine
