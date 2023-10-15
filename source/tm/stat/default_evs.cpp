// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.default_evs;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.generation;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

// Used only in tests
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

// Used only in tests and SeenPokemon. This file can be made test-only when
// SeenPokemon is updated to just track direct observations instead of being
// basically a Pokemon.
export template<Generation generation>
constexpr auto default_combined_stats = CombinedStatsFor<generation>{
	Nature::Hardy,
	max_dvs_or_ivs<generation>,
	default_evs<generation>
};

} // namespace technicalmachine
