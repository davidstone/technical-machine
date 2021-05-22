// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/stat/iv.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/nature.hpp>

namespace technicalmachine {

template<Generation generation>
struct CombinedStats {
	Nature nature;
	IVs dvs_or_ivs;
	EVs evs;

	friend auto operator==(CombinedStats, CombinedStats) -> bool = default;
};

// Simplifies unit tests
template<Generation generation>
inline constexpr auto default_evs = [] {
	if constexpr (generation <= Generation::two) {
		constexpr auto ev = EV(252_bi);
		return EVs(ev, ev, ev, ev, ev, ev);
	} else {
		constexpr auto ev = EV(0_bi);
		return EVs(ev, ev, ev, ev, ev, ev);
	}
}();

// Simplifies unit tests
template<Generation generation>
inline constexpr auto default_combined_stats = CombinedStats<generation>{
	Nature::Hardy,
	max_dvs_or_ivs<generation>,
	default_evs<generation>
};

} // namespace technicalmachine
