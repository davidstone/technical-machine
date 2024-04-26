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
import tm.stat.nature_effect;
import tm.stat.stat_style;

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

export constexpr auto stat_to_ev_at_least_pre_iv(
	bounded::bounded_integer auto const target,
	bounded::bounded_integer auto const base,
	Level const level,
	NatureEffect const nature_effect
) {
	auto const value = round_up_divide((round_up_divide(target, boost(nature_effect)) - 5_bi) * 100_bi, level()) - 2_bi * base;
	return [=](IV const iv) -> tv::optional<EV> {
		return maybe_ev((value - iv.value()) * 4_bi);
	};
}

export template<SpecialStyle stat_style>
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, bounded::bounded_integer auto const base, Level const level, NatureEffect const nature_effect, IV const iv) -> tv::optional<EV> {
	auto const ev = stat_to_ev_at_least_pre_iv(target, base, level, nature_effect)(iv);
	if (!ev) {
		return tv::none;
	}
	if (initial_stat<stat_style>(base, level, nature_effect, iv, *ev) != target) {
		return tv::none;
	}
	return ev;
}


static_assert(hp_to_ev(364_bi, 80_bi, Level(100_bi), IV(31_bi)) == EV(252_bi));
static_assert(hp_to_ev(257_bi, 78_bi, Level(78_bi), IV(29_bi)) == EV(128_bi));

static_assert(stat_to_ev<SpecialStyle::split>(133_bi, 70_bi, Level(100_bi), NatureEffect::negative, IV(3_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(177_bi, 65_bi, Level(100_bi), NatureEffect::neutral, IV(31_bi)) == EV(44_bi));
static_assert(stat_to_ev<SpecialStyle::split>(196_bi, 80_bi, Level(100_bi), NatureEffect::neutral, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(352_bi, 120_bi, Level(100_bi), NatureEffect::positive, IV(31_bi)) == EV(176_bi));
static_assert(stat_to_ev<SpecialStyle::split>(245_bi, 100_bi, Level(100_bi), NatureEffect::neutral, IV(31_bi)) == EV(36_bi));
static_assert(stat_to_ev<SpecialStyle::split>(614_bi, 230_bi, Level(100_bi), NatureEffect::positive, IV(31_bi)) == EV(252_bi));
static_assert(stat_to_ev<SpecialStyle::split>(558_bi, 230_bi, Level(100_bi), NatureEffect::neutral, IV(DV(15_bi))) == EV(252_bi));
static_assert(stat_to_ev<SpecialStyle::split>(178_bi, 125_bi, Level(63_bi), NatureEffect::negative, IV(19_bi)) == EV(152_bi));
static_assert(stat_to_ev<SpecialStyle::split>(96_bi, 84_bi, Level(37_bi), NatureEffect::positive, IV(13_bi)) == EV(176_bi));
static_assert(stat_to_ev<SpecialStyle::split>(100_bi, 70_bi, Level(75_bi), NatureEffect::negative, IV(3_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(236_bi, 125_bi, Level(75_bi), NatureEffect::positive, IV(31_bi)) == EV(0_bi));

static_assert(hp_to_ev(251_bi, 55_bi, Level(100_bi), IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(359_bi, 130_bi, Level(100_bi), NatureEffect::neutral, IV(31_bi)) == EV(252_bi));
static_assert(stat_to_ev<SpecialStyle::split>(265_bi, 115_bi, Level(100_bi), NatureEffect::neutral, IV(30_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(122_bi, 50_bi, Level(100_bi), NatureEffect::negative, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(136_bi, 50_bi, Level(100_bi), NatureEffect::neutral, IV(30_bi)) == EV(4_bi));
static_assert(stat_to_ev<SpecialStyle::split>(273_bi, 75_bi, Level(100_bi), NatureEffect::positive, IV(31_bi)) == EV(252_bi));

static_assert(hp_to_ev(12_bi, 60_bi, Level(1_bi), IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(6_bi, 65_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi)) == EV(0_bi));
static_assert(stat_to_ev<SpecialStyle::split>(7_bi, 60_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi)) == EV(196_bi));
static_assert(stat_to_ev<SpecialStyle::split>(8_bi, 130_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi)) == EV(36_bi));
static_assert(stat_to_ev<SpecialStyle::split>(7_bi, 75_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi)) == EV(76_bi));
static_assert(stat_to_ev<SpecialStyle::split>(8_bi, 110_bi, Level(1_bi), NatureEffect::neutral, IV(31_bi)) == EV(196_bi));

} // namespace technicalmachine
