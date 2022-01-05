// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <limits>
#include <stdexcept>
#include <string>

namespace technicalmachine::ps_usage_stats {

struct Rating {
	static constexpr auto initial_value = 1500.0;
	static constexpr auto initial_deviation = 350.0;

	constexpr Rating(double const value_, double const deviation_):
		value(value_),
		deviation(deviation_)
	{
		auto const value_in_range = std::numeric_limits<double>::min() < value_ and value <= std::numeric_limits<double>::max();
		if (!value_in_range) {
			throw std::runtime_error("Invalid rating value of " + std::to_string(value_));
		}
		auto const deviation_in_range = std::numeric_limits<double>::min() < deviation_ and deviation_ <= initial_deviation;
		if (!deviation_in_range) {
			throw std::runtime_error("Invalid rating deviation of " + std::to_string(deviation_));
		}
	}

	double value;
	double deviation;
	friend auto operator==(Rating, Rating) -> bool = default;
};

inline constexpr auto pi = 3.14159265358979323846;

inline constexpr auto square(double const value) {
	return value * value;
}

auto chance_to_win(Rating) -> double;

} // namespace technicalmachine::ps_usage_stats
