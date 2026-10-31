export module tm.stat.default_evs;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.stat_style;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export template<SpecialInputStyle style>
constexpr auto default_evs = [] {
	if constexpr (style == SpecialInputStyle::combined) {
		constexpr auto ev = EV(252_bi);
		return OldGenEVs(ev, ev, ev, ev, ev);
	} else {
		constexpr auto ev = EV(0_bi);
		return EVs(ev, ev, ev, ev, ev, ev);
	}
}();

} // namespace technicalmachine
