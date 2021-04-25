// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>
#include <bounded/clamp.hpp>

#include <numeric_traits/min_max_value.hpp>

namespace technicalmachine {

using namespace bounded::literal;

template<bounded::bounded_integer Integer>
constexpr auto saturating_add(Integer & x, bounded::bounded_integer auto other) {
	x = bounded::clamp(x + other, numeric_traits::min_value<Integer>, numeric_traits::max_value<Integer>);
}

constexpr auto saturating_increment(bounded::bounded_integer auto & x) {
	saturating_add(x, 1_bi);
}

constexpr auto saturating_decrement(bounded::bounded_integer auto & x) {
	saturating_add(x, -1_bi);
}

} // namespace technicalmachine
