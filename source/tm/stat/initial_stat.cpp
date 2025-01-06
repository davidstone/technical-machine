// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.initial_stat;

import tm.pokemon.level;

import tm.stat.base_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature_effect;
import tm.stat.stat_style;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto initial_stat(
	auto const base,
	Level const level,
	IV const iv,
	EV const ev
) {
	return (2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 5_bi;
}

export constexpr auto initial_stat(
	auto const base,
	Level const level,
	NatureEffect const nature_effect,
	IV const iv,
	EV const ev
) {
	return initial_stat(base, level, iv, ev) * boost(nature_effect);
}

export template<SpecialInputStyle stat_style>
using InitialStat = std::conditional_t<
	stat_style == SpecialInputStyle::combined,
	decltype(initial_stat(
		bounded::declval<BaseStats::regular_value_type>(),
		bounded::declval<Level>(),
		bounded::declval<IV>(),
		bounded::declval<EV>()
	)),
	decltype(initial_stat(
		bounded::declval<BaseStats::regular_value_type>(),
		bounded::declval<Level>(),
		bounded::declval<NatureEffect>(),
		bounded::declval<IV>(),
		bounded::declval<EV>()
	))
>;


static_assert(initial_stat(100_bi, Level(100_bi), NatureEffect::neutral, IV(31_bi), EV(252_bi)) == 299_bi);
static_assert(initial_stat(65_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi), EV(0_bi)) == 6_bi);
static_assert(initial_stat(60_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi), EV(196_bi)) == 7_bi);
static_assert(initial_stat(130_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi), EV(36_bi)) == 8_bi);
static_assert(initial_stat(75_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi), EV(76_bi)) == 7_bi);
static_assert(initial_stat(110_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi), EV(196_bi)) == 8_bi);

} // namespace technicalmachine