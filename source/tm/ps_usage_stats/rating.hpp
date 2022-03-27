// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

namespace technicalmachine::ps_usage_stats {

// https://www.smogon.com/forums/threads/gxe-glixare-a-much-better-way-of-estimating-a-players-overall-rating-than-shoddys-cre.51169/
// https://pokemonshowdown.com/pages/ladderhelp

struct Rating {
	static constexpr auto initial_value = 1500.0;
	static constexpr auto initial_deviation = 130.0;

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

inline constexpr auto initial_rating = Rating(Rating::initial_value, Rating::initial_deviation);

inline constexpr auto pi = 3.14159265358979323846;

inline constexpr auto square(double const value) {
	return value * value;
}

inline auto chance_to_win(Rating const player, Rating const opponent) -> double {
	auto const c = 3.0 * square(std::log(10.0) / (400.0 * pi));
	auto const rating_difference = opponent.value - player.value;
	auto const deviation_factor = square(player.deviation) + square(opponent.deviation);
	return 1.0 / (1.0 + std::pow(10.0, (rating_difference / (400.0 * std::sqrt(1.0 + c * deviation_factor)))));
}

} // namespace technicalmachine::ps_usage_stats
