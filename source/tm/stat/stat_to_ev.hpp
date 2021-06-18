// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/initial_stat.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/iv_and_ev.hpp>
#include <tm/stat/nature.hpp>
#include <tm/stat/stat_names.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>
#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/generation.hpp>
#include <tm/string_conversions/species.hpp>

#include <bounded/optional.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/integer_range.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;
using namespace bounded::literal;
using namespace std::string_view_literals;

template<typename RHS>
constexpr auto round_up_divide(bounded::bounded_integer auto const lhs, RHS const rhs) {
	if constexpr (bounded::bounded_integer<RHS>) {
		return (lhs + rhs - 1_bi) / rhs;
	} else {
		return round_up_divide(lhs * rhs.denominator(), rhs.numerator());
	}
}

constexpr auto hp_to_ev(BaseStats::HP const base, Level const level, HP::max_type const stat, IV const iv) -> EV {
	if (base == 1_bi) {
		return EV(0_bi);
	}
	auto const computed = (round_up_divide((stat - level() - 10_bi) * 100_bi, level()) - iv.value() - 2_bi * base) * 4_bi;
	return EV(EV::value_type(bounded::max(0_bi, computed)));
}

// `target` is not just InitialStat because this function is also used in the EV
// optimizer, where values outside the legal range are regularly encountered as
// part of speculative computation.
constexpr auto stat_to_ev(bounded::bounded_integer auto const target, SplitSpecialRegularStat const stat_name, bounded::bounded_integer auto const base, Level const level, Nature const nature, IV const iv) -> bounded::optional<EV> {
	auto const computed = (round_up_divide((round_up_divide(target, boost(nature, stat_name)) - 5_bi) * 100_bi, level()) - 2_bi * base - iv.value()) * 4_bi;
	if (computed > EV::max) {
		return bounded::none;
	}
	return EV(EV::value_type(bounded::max(0_bi, computed), bounded::non_check));
}

} // namespace technicalmachine
