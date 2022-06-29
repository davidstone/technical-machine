// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/level.hpp>

#include <tm/round_up_divide.hpp>

#include <bounded/optional.hpp>

#include <containers/integer_range.hpp>

namespace technicalmachine {

using namespace bounded::literal;

namespace detail {

constexpr auto maybe_ev(bounded::bounded_integer auto const computed) -> bounded::optional<EV> {
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::unchecked));
}

} // namespace detail

constexpr auto hp_to_ev(HP::max_type const target, BaseStats::HP const base, Level const level, IV const iv) -> bounded::optional<EV> {
	if (base == 1_bi) {
		if (target != 1_bi) {
			return bounded::none;
		}
		return EV(0_bi);
	}
	auto const ev = detail::maybe_ev((round_up_divide((target - level() - 10_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi);
	if (!ev) {
		return bounded::none;
	}
	if (HP(base, level, iv, *ev).max() != target) {
		return bounded::none;
	}
	return ev;
}

constexpr auto stat_to_ev_at_least(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	return detail::maybe_ev(computed);
}

template<Generation generation>
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const ev = stat_to_ev_at_least(target, stat_name, base, level, nature, iv);
	if (!ev) {
		return bounded::none;
	}
	if (initial_stat<generation>(stat_name, base, level, nature, iv, *ev) != target) {
		return bounded::none;
	}
	return ev;
}

} // namespace technicalmachine
