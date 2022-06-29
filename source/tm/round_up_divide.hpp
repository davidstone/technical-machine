// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

namespace technicalmachine {

using namespace bounded::literal;

constexpr auto round_up_divide(bounded::bounded_integer auto const lhs, bounded::bounded_integer auto const rhs) {
	return (lhs + rhs - 1_bi) / rhs;
}

} // namespace technicalmachine
