// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/stat/stat_to_ev.hpp>

namespace technicalmachine {
namespace {

static_assert(round_up_divide(1_bi, 2_bi) == 1_bi);
static_assert(round_up_divide(1_bi, 1_bi) == 1_bi);
static_assert(round_up_divide(5_bi, 1_bi) == 5_bi);
static_assert(round_up_divide(6_bi, 5_bi) == 2_bi);

static_assert(stat_to_ev(614_bi, Nature::Impish, RegularStat::def, 230_bi, IV(31_bi), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(558_bi, Nature::Hardy, RegularStat::def, 230_bi, IV(DV(15_bi)), Level(100_bi)) == EV(252_bi));
static_assert(stat_to_ev(178_bi, Nature::Bold, RegularStat::atk, 125_bi, IV(19_bi), Level(63_bi)) == EV(152_bi));

} // namespace
} // namespace technicalmachine
