// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace technicalmachine::ps_usage_stats {

struct Rating {
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
