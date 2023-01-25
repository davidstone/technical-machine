// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.initial_stat;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.stat_names;
import tm.stat.nature;

import tm.generation;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
constexpr auto initial_stat(SplitSpecialRegularStat const stat_name, auto const base, Level const level, Nature const nature, IV const iv, EV const ev) {
	auto const pre_nature = (2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 5_bi;
	if constexpr (generation <= Generation::two) {
		return pre_nature;
	} else {
		return pre_nature * boost(nature, stat_name);
	}
}

export template<Generation generation>
using InitialStat = decltype(initial_stat<generation>(
	bounded::declval<SplitSpecialRegularStat>(),
	bounded::declval<BaseStats::regular_value_type>(),
	bounded::declval<Level>(),
	bounded::declval<Nature>(),
	bounded::declval<IV>(),
	bounded::declval<EV>()
));


static_assert(initial_stat<Generation::three>(SplitSpecialRegularStat::spe, 100_bi, Level(100_bi), Nature::Hardy, IV(31_bi), EV(252_bi)) == 299_bi);

static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::atk, 65_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(0_bi)) == 6_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::def, 60_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(196_bi)) == 7_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spa, 130_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(36_bi)) == 8_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spd, 75_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(76_bi)) == 7_bi);
static_assert(initial_stat<Generation::four>(SplitSpecialRegularStat::spe, 110_bi, Level(1_bi), Nature::Hardy, IV(31_bi), EV(196_bi)) == 8_bi);

} // namespace technicalmachine