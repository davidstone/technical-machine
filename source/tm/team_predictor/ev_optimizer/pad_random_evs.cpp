// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <vector>

export module tm.team_predictor.ev_optimizer.pad_random_evs;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.stat_names;

import tm.generation;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export template<Generation generation>
constexpr auto pad_random_evs(CombinedStats<generation> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<generation> {
	if constexpr (generation <= Generation::two) {
		for (auto const stat_name : containers::enum_range<SpecialPermanentStat>()) {
			auto const minimize_stat = stat_name == SpecialPermanentStat::atk and !include_attack;
			combined.evs[stat_name] = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
		}
	} else {
		auto distribution = std::discrete_distribution();
		constexpr auto minimal_increment = 4_bi;
		while (ev_sum(combined.evs) + minimal_increment <= max_total_evs(generation)) {
			distribution.param({
				combined.evs.hp() >= EV::useful_max ? 0.0 : 1.0,
				(!include_attack or combined.evs.atk() >= EV::useful_max) ? 0.0 : 1.0,
				combined.evs.def() >= EV::useful_max ? 0.0 : 1.0,
				(!include_special_attack or combined.evs.spa() >= EV::useful_max) ? 0.0 : 1.0,
				combined.evs.spd() >= EV::useful_max ? 0.0 : 1.0,
				combined.evs.spe() >= EV::useful_max ? 0.0 : 1.0,
			});
			auto const index = distribution(random_engine);
			auto & ev = combined.evs[SplitSpecialPermanentStat(index - 1)];
			ev = EV(::bounded::assume_in_range<EV::value_type>(ev.value() + minimal_increment));
		}
	}
	return combined;
}

} // namespace technicalmachine
