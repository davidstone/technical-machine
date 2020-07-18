// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/team_predictor/estimate.hpp>

#include <tm/team_predictor/usage_stats.hpp>

#include <tm/team.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/find.hpp>
#include <containers/integer_range.hpp>

#include <algorithm>
#include <cstddef>
#include <random>

namespace technicalmachine {

Estimate::Estimate(UsageStats const & usage_stats, LeadStats const lead_stats) {
	auto const & lead = lead_stats.get(usage_stats);
	auto const total = usage_stats.total_weighted_usage();
	for (auto const species : containers::enum_range<Species>()) {
		m_estimate[bounded::integer(species)] = lead[bounded::integer(species)] * usage_stats.get(species).weighted_usage / static_cast<value_type>(total);
	}
}

void Estimate::update(UsageStats const & usage_stats, Species const seen) {
	for (auto const predicted : containers::enum_range<Species>()) {
		m_estimate[bounded::integer(predicted)] *= usage_stats.get(seen).teammates[bounded::integer(predicted)];
	}
}

Species Estimate::most_likely() const {
	auto const it = std::max_element(begin(m_estimate), end(m_estimate));
	// Estimate is indexed by Species ID. The Species ID is therefore the
	// distance between the found element and the beginning.
	return static_cast<Species>(it - begin(m_estimate));
}

Species Estimate::random(std::mt19937 & random_engine) const {
	auto const total = containers::sum(m_estimate);
	auto distribution = std::uniform_real_distribution<float>(0.0F, total);
	auto usage_threshold = distribution(random_engine);
	auto const it = containers::find_if(m_estimate, [=](auto const value) mutable {
		usage_threshold -= value;
		return usage_threshold <= 0.0F;
	});
	BOUNDED_ASSERT(it != end(m_estimate));
	return static_cast<Species>(it - begin(m_estimate));
}

}	// namespace technicalmachine
