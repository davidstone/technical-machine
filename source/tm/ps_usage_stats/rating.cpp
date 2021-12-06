// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/rating.hpp>

#include <cmath>

namespace technicalmachine::ps_usage_stats {
namespace {

constexpr auto square(double const value) {
	return value * value;
}

} // namespace

auto chance_to_win(Rating const rating) -> double {
	// https://www.smogon.com/forums/threads/gxe-glixare-a-much-better-way-of-estimating-a-players-overall-rating-than-shoddys-cre.51169/
	if (rating.deviation > 100.0) {
		return 0.0;
	}
	constexpr auto pi = 3.14159265358979323846;
	return 1.0 / (1.0 + std::pow(10.0, ((1500.0 - rating.value) * pi / std::sqrt(3.0 * square(std::log(10.0)) * square(rating.deviation) + 2500.0 * (64.0 * square(pi) + 147.0 * square(std::log(10.0)))))));
}

} // namespace technicalmachine::ps_usage_stats
