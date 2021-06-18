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

static_assert(stat_to_ev(614_bi, SplitSpecialRegularStat::def, 230_bi, Level(100_bi), Nature::Impish, IV(31_bi)) == EV(252_bi));
static_assert(stat_to_ev(558_bi, SplitSpecialRegularStat::def, 230_bi, Level(100_bi), Nature::Hardy, IV(DV(15_bi))) == EV(252_bi));
static_assert(stat_to_ev(178_bi, SplitSpecialRegularStat::atk, 125_bi, Level(63_bi), Nature::Bold, IV(19_bi)) == EV(152_bi));

static_assert([] {
	constexpr auto const base_hp = 78_bi;
	constexpr auto level = Level(78_bi);
	constexpr auto iv = IV(29_bi);
	constexpr auto ev = EV(128_bi);
	constexpr auto hp = HP(base_hp, level, iv, ev).max();
	constexpr auto computed_ev = hp_to_ev(base_hp, level, hp, iv);
	return computed_ev == ev;
}());

static_assert([] {
	constexpr auto generation = Generation::four;
	constexpr auto base_atk = 84_bi;
	constexpr auto level = Level(37_bi);
	constexpr auto iv = IV(13_bi);
	constexpr auto ev = EV(176_bi);
	constexpr auto nature = Nature::Adamant;
	constexpr auto stat_name = SplitSpecialRegularStat::atk;
	constexpr auto stat_value = initial_stat<generation>(stat_name, base_atk, level, nature, iv, ev);
	constexpr auto computed_ev = stat_to_ev(stat_value, stat_name, base_atk, level, nature, iv);
	return computed_ev == ev;
}());

} // namespace
} // namespace technicalmachine
