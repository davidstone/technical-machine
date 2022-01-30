// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/species.hpp>

#include <tm/number_of.hpp>

#include <containers/array.hpp>

#include <random>

namespace technicalmachine {

struct UsageStats;

struct Estimate {
	explicit Estimate(UsageStats const & usage_stats);
	void update(UsageStats const & usage_stats, Species seen);
	Species most_likely() const;
	Species random(std::mt19937 & random_engine) const;
private:
	containers::array<float, number_of<Species>> m_estimate;
};

}	// namespace technicalmachine
