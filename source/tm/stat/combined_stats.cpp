// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.combined_stats;

import tm.stat.iv;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.nature;

import tm.generation;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
struct CombinedStats {
	Nature nature;
	DVsOrIVs<generation> dvs_or_ivs;
	std::conditional_t<generation <= Generation::two, OldGenEVs, EVs> evs;
	friend auto operator==(CombinedStats, CombinedStats) -> bool = default;
};

// Simplifies unit tests
export template<Generation generation>
constexpr auto default_evs = [] {
	if constexpr (generation <= Generation::two) {
		constexpr auto ev = EV(252_bi);
		return OldGenEVs(ev, ev, ev, ev, ev);
	} else {
		constexpr auto ev = EV(0_bi);
		return EVs(ev, ev, ev, ev, ev, ev);
	}
}();

// Simplifies unit tests
export template<Generation generation>
constexpr auto default_combined_stats = CombinedStats<generation>{
	Nature::Hardy,
	max_dvs_or_ivs<generation>,
	default_evs<generation>
};

} // namespace technicalmachine
