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

constexpr auto generation = Generation::four;

static_assert(hp_to_ev(364_bi, 80_bi, Level(100_bi), IV(31_bi)) == EV(252_bi));
static_assert(hp_to_ev(257_bi, 78_bi, Level(78_bi), IV(29_bi)) == EV(128_bi));

static_assert(stat_to_ev<generation>(133_bi, SplitSpecialRegularStat::atk, 70_bi, Level(100_bi), Nature::Calm, IV(3_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(177_bi, SplitSpecialRegularStat::def, 65_bi, Level(100_bi), Nature::Calm, IV(31_bi)) == EV(44_bi));
static_assert(stat_to_ev<generation>(196_bi, SplitSpecialRegularStat::spa, 80_bi, Level(100_bi), Nature::Calm, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(352_bi, SplitSpecialRegularStat::spd, 120_bi, Level(100_bi), Nature::Calm, IV(31_bi)) == EV(176_bi));
static_assert(stat_to_ev<generation>(245_bi, SplitSpecialRegularStat::spe, 100_bi, Level(100_bi), Nature::Calm, IV(31_bi)) == EV(36_bi));
static_assert(stat_to_ev<generation>(614_bi, SplitSpecialRegularStat::def, 230_bi, Level(100_bi), Nature::Impish, IV(31_bi)) == EV(252_bi));
static_assert(stat_to_ev<generation>(558_bi, SplitSpecialRegularStat::def, 230_bi, Level(100_bi), Nature::Hardy, IV(DV(15_bi))) == EV(252_bi));
static_assert(stat_to_ev<generation>(178_bi, SplitSpecialRegularStat::atk, 125_bi, Level(63_bi), Nature::Bold, IV(19_bi)) == EV(152_bi));
static_assert(stat_to_ev<generation>(96_bi, SplitSpecialRegularStat::atk, 84_bi, Level(37_bi), Nature::Adamant, IV(13_bi)) == EV(176_bi));
static_assert(stat_to_ev<generation>(100_bi, SplitSpecialRegularStat::atk, 70_bi, Level(75_bi), Nature::Modest, IV(3_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(236_bi, SplitSpecialRegularStat::spa, 125_bi, Level(75_bi), Nature::Modest, IV(31_bi)) == EV(0_bi));

static_assert(hp_to_ev(251_bi, 55_bi, Level(100_bi), IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(359_bi, SplitSpecialRegularStat::atk, 130_bi, Level(100_bi), Nature::Jolly, IV(31_bi)) == EV(252_bi));
static_assert(stat_to_ev<generation>(265_bi, SplitSpecialRegularStat::def, 115_bi, Level(100_bi), Nature::Jolly, IV(30_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(122_bi, SplitSpecialRegularStat::spa, 50_bi, Level(100_bi), Nature::Jolly, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(136_bi, SplitSpecialRegularStat::spd, 50_bi, Level(100_bi), Nature::Jolly, IV(30_bi)) == EV(4_bi));
static_assert(stat_to_ev<generation>(273_bi, SplitSpecialRegularStat::spe, 75_bi, Level(100_bi), Nature::Jolly, IV(31_bi)) == EV(252_bi));

static_assert(hp_to_ev(12_bi, 60_bi, Level(1_bi), IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(6_bi, SplitSpecialRegularStat::atk, 65_bi, Level(1_bi), Nature::Hardy, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<generation>(7_bi, SplitSpecialRegularStat::def, 60_bi, Level(1_bi), Nature::Hardy, IV(31_bi)) == EV(196_bi));
static_assert(stat_to_ev<generation>(8_bi, SplitSpecialRegularStat::spa, 130_bi, Level(1_bi), Nature::Hardy, IV(31_bi)) == EV(36_bi));
static_assert(stat_to_ev<generation>(7_bi, SplitSpecialRegularStat::spd, 75_bi, Level(1_bi), Nature::Hardy, IV(31_bi)) == EV(76_bi));
static_assert(stat_to_ev<generation>(8_bi, SplitSpecialRegularStat::spe, 110_bi, Level(1_bi), Nature::Hardy, IV(31_bi)) == EV(196_bi));

} // namespace
} // namespace technicalmachine
