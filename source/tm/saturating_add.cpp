// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.saturating_add;

import bounded;
import numeric_traits;

namespace technicalmachine {

using namespace bounded::literal;

export template<bounded::bounded_integer Integer>
constexpr auto saturating_add(Integer & x, bounded::bounded_integer auto other) {
	x = bounded::clamp(x + other, numeric_traits::min_value<Integer>, numeric_traits::max_value<Integer>);
}

export constexpr auto saturating_increment(bounded::bounded_integer auto & x) {
	saturating_add(x, 1_bi);
}

export constexpr auto saturating_decrement(bounded::bounded_integer auto & x) {
	saturating_add(x, -1_bi);
}

} // namespace technicalmachine
