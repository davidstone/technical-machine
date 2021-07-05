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


#include <bounded/optional.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>

namespace technicalmachine {

using namespace bounded::literal;

template<typename RHS>
constexpr auto round_up_divide(bounded::bounded_integer auto const lhs, RHS const rhs) {
	if constexpr (bounded::bounded_integer<RHS>) {
		return (lhs + rhs - 1_bi) / rhs;
	} else {
		return round_up_divide(lhs * rhs.denominator(), rhs.numerator());
	}
}

namespace detail {

template<typename Target, bounded::bounded_integer Source>
constexpr auto maybe_integer(Source const source) -> bounded::optional<Target> {
	if (source < numeric_traits::min_value<Target> or numeric_traits::max_value<Target> < source) {
		return bounded::none;
	}
	return Target(source, bounded::non_check);
}

constexpr auto maybe_ev(auto const value) -> bounded::optional<EV> {
	auto const ev = maybe_integer<EV::value_type>(value);
	if (!ev) {
		return bounded::none;
	}
	return EV(*ev);
}

} // namespace detail

constexpr auto hp_to_ev(HP::max_type const target, BaseStats::HP const base, Level const level, IV const iv) -> bounded::optional<EV> {
	if (base == 1_bi) {
		return EV(0_bi);
	}
	return detail::maybe_ev((round_up_divide((stat - level() - 10_bi) * 100_bi, level()) - iv.value() - 2_bi * base) * 4_bi);
}

constexpr auto stat_to_ev_at_least(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::non_check));
}

template<Generation generation>
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const result = stat_to_ev_at_least(target, stat_name, base, level, nature, iv);
	if (!result) {
		return bounded::none;
	}
	if (initial_stat<generation>(stat_name, base, level, nature, iv, *result) != target) {
		return bounded::none;
	}
	return result;
}

} // namespace technicalmachine
