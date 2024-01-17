// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.stat.default_evs;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.stat_style;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export template<SpecialStyle style>
constexpr auto default_evs = [] {
	if constexpr (style == SpecialStyle::combined) {
		constexpr auto ev = EV(252_bi);
		return OldGenEVs(ev, ev, ev, ev, ev);
	} else {
		constexpr auto ev = EV(0_bi);
		return EVs(ev, ev, ev, ev, ev, ev);
	}
}();

} // namespace technicalmachine
