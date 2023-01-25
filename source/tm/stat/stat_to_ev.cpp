// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.stat_to_ev;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.hp;
import tm.stat.initial_stat;
import tm.stat.iv;
import tm.stat.max_hp;
import tm.stat.nature;
import tm.stat.stat_names;

import tm.generation;
import tm.round_up_divide;

import bounded;
import containers;
import tv;

namespace technicalmachine {

using namespace bounded::literal;

constexpr auto maybe_ev(bounded::bounded_integer auto const computed) -> tv::optional<EV> {
	if (computed > EV::max) {
		return tv::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::unchecked));
}

export constexpr auto hp_to_ev(MaxHP const target, BaseStats::HP const base, Level const level, IV const iv) -> tv::optional<EV> {
	if (base == 1_bi) {
		if (target != 1_bi) {
			return tv::none;
		}
		return EV(0_bi);
	}
	auto const ev = maybe_ev((round_up_divide((target - level() - 10_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi);
	if (!ev) {
		return tv::none;
	}
	if (HP(base, level, iv, *ev).max() != target) {
		return tv::none;
	}
	return ev;
}

export constexpr auto stat_to_ev_at_least(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> tv::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	return maybe_ev(computed);
}

export template<Generation generation>
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> tv::optional<EV> {
	auto const ev = stat_to_ev_at_least(target, stat_name, base, level, nature, iv);
	if (!ev) {
		return tv::none;
	}
	if (initial_stat<generation>(stat_name, base, level, nature, iv, *ev) != target) {
		return tv::none;
	}
	return ev;
}


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

} // namespace technicalmachine
