// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/level.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<Generation generation>
constexpr auto initial_stat(RegularStat const stat_name, auto const base, Level const level, Nature const nature, IV const iv, EV const ev) {
	auto const pre_nature = (2_bi * base + iv.value() + ev.value() / 4_bi) * level() / 100_bi + 5_bi;
	if constexpr (generation <= Generation::two) {
		return pre_nature;
	} else {
		return pre_nature * boost(nature, stat_name);
	}
}

template<Generation generation>
using InitialStat = decltype(initial_stat<generation>(
	std::declval<RegularStat>(),
	std::declval<BaseStats::regular_value_type>(),
	std::declval<Level>(),
	std::declval<Nature>(),
	std::declval<IV>(),
	std::declval<EV>()
));

} // namespace technicalmachine
