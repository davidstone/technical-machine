// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.stat.evs;

import tm.stat.generic_stats;
import tm.stat.ev;
import tm.stat.stat_style;

import bounded;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export using EVs = GenericStats<EV>;
export using OldGenEVs = GenericCombinedStats<EV>;

export constexpr auto to_old_gen_evs(EVs const evs) -> OldGenEVs {
	if (evs.spa != evs.spd) {
		throw std::runtime_error("Mismatched SpA and SpD EVs in old generation");
	}
	return OldGenEVs(evs.hp, evs.atk, evs.def, evs.spe, evs.spa);
}

export constexpr auto max_total_evs(SpecialInputStyle const stat_style) {
	return BOUNDED_CONDITIONAL(
		stat_style == SpecialInputStyle::combined,
		EV::max * 5_bi,
		510_bi
	);
}

export constexpr auto ev_sum(EVs const evs) {
	return
		evs.hp.value() +
		evs.atk.value() +
		evs.def.value() +
		evs.spa.value() +
		evs.spd.value() +
		evs.spe.value();
}
export constexpr auto ev_sum(OldGenEVs const evs) {
	return
		evs.hp.value() +
		evs.atk.value() +
		evs.def.value() +
		evs.spe.value() +
		evs.spc.value();
}

} // namespace technicalmachine
