// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/initial_stat.hpp>

namespace technicalmachine {
namespace {

static_assert(initial_stat<Generation::three>(SplitSpecialRegularStat::spe, 100_bi, Level(100_bi), Nature::Hardy, IV(31_bi), EV(252_bi)) == 299_bi);

static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::atk, 65_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(0_bi)) == 6_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::def, 60_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(196_bi)) == 7_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spa, 130_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(36_bi)) == 8_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spd, 75_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(76_bi)) == 7_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spe, 110_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(196_bi)) == 8_bi);

} // namespace
} // namespace technicalmachine