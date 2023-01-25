// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.round_up_divide;

import bounded;

namespace technicalmachine {

using namespace bounded::literal;

export constexpr auto round_up_divide(bounded::bounded_integer auto const lhs, bounded::bounded_integer auto const rhs) {
	return (lhs + rhs - 1_bi) / rhs;
}

namespace {

static_assert(round_up_divide(1_bi, 2_bi) == 1_bi);
static_assert(round_up_divide(1_bi, 1_bi) == 1_bi);
static_assert(round_up_divide(5_bi, 1_bi) == 5_bi);
static_assert(round_up_divide(6_bi, 5_bi) == 2_bi);

} // namespace
} // namespace technicalmachine
