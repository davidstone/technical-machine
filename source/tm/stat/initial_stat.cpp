// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/initial_stat.hpp>

namespace technicalmachine {
namespace {

static_assert(initial_stat<Generation::three>(SplitSpecialRegularStat::spe, 100_bi, Level(100_bi), Nature::Hardy, IV(31_bi), EV(252_bi)) == 299_bi);

} // namespace
} // namespace technicalmachine