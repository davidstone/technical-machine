// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/hp.hpp>

namespace technicalmachine {
namespace {

static_assert(HP(100_bi).max() == 100_bi);
static_assert(HP(100_bi).current() == 100_bi);
static_assert(HP(60_bi, Level(1_bi), IV(31_bi), EV(0_bi)).max() == 12_bi);

} // namespace
} // namespace technicalmachine