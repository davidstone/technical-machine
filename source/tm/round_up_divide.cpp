// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/round_up_divide.hpp>

namespace technicalmachine {
namespace {

static_assert(round_up_divide(1_bi, 2_bi) == 1_bi);
static_assert(round_up_divide(1_bi, 1_bi) == 1_bi);
static_assert(round_up_divide(5_bi, 1_bi) == 5_bi);
static_assert(round_up_divide(6_bi, 5_bi) == 2_bi);

} // namespace
} // namespace technicalmachine
