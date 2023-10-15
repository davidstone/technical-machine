// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <vector>

export module tm.team_predictor.ev_optimizer.pad_random_evs;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.stat_names;
import tm.stat.stat_style;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto pad_random_evs_combined(CombinedStats<SpecialStyle::combined> combined, bool const include_attack) -> CombinedStats<SpecialStyle::combined> {
	for (auto const stat_name : containers::enum_range<SpecialPermanentStat>()) {
		auto const minimize_stat = stat_name == SpecialPermanentStat::atk and !include_attack;
		combined.evs[stat_name] = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
	}
	return combined;
}

auto pad_random_evs_split(CombinedStats<SpecialStyle::split> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<SpecialStyle::split> {
	auto distribution = std::discrete_distribution();
	constexpr auto minimal_increment = 4_bi;
	while (ev_sum(combined.evs) + minimal_increment <= max_total_evs(SpecialStyle::split)) {
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
	return combined;
}

export template<SpecialStyle stat_style>
constexpr auto pad_random_evs(CombinedStats<stat_style> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<stat_style> {
	if constexpr (stat_style == SpecialStyle::combined) {
		return pad_random_evs_combined(combined, include_attack);
	} else {
		return pad_random_evs_split(combined, include_attack, include_special_attack, random_engine);
	}
}

} // namespace technicalmachine
