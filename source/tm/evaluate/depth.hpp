// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/assume_in_range.hpp>
#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

#include <compare>
#include <utility>

namespace technicalmachine {

using namespace bounded::literal;

using DepthInt = bounded::integer<0, 10'000>;

struct Depth {
	constexpr Depth(DepthInt const general_, DepthInt const single_):
		general(general_),
		single(single_)
	{
	}
	DepthInt general;
	DepthInt single;
	friend constexpr auto operator<=>(Depth, Depth) = default;
};

constexpr auto one_level_deeper(Depth depth) -> Depth {
	if (depth.general > 0_bi) {
		--depth.general;
	} else if (depth.single > 0_bi) {
		--depth.single;
	} else {
		std::unreachable();
	}
	return depth;
}

constexpr auto iterative_deepening_value(Depth const depth) {
	return BOUNDED_CONDITIONAL(
		depth.general > 1_bi,
		Depth(bounded::assume_in_range<DepthInt>(depth.general - 1_bi), depth.single),
		bounded::none
	);
}


} // namespace technicalmachine
