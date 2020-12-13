// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/lead_stats.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <containers/array/array.hpp>

#include <random>

namespace technicalmachine {

struct UsageStats;

struct Estimate {
	Estimate(UsageStats const & usage_stats, LeadStats lead_stats);
	void update(UsageStats const & usage_stats, Species seen);
	Species most_likely() const;
	Species random(std::mt19937 & random_engine) const;
private:
	using value_type = float;
	containers::array<value_type, number_of_species> m_estimate;
};

}	// namespace technicalmachine
