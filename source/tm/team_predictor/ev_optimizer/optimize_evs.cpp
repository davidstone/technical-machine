// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.ev_optimizer.optimize_evs;

import tm.team_predictor.ev_optimizer.compute_minimal_spread;
import tm.team_predictor.ev_optimizer.pad_evs;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.stat_style;
import tm.stat.stats;

import tm.generation;

import tv;

namespace technicalmachine {

export template<Generation generation>
auto optimize_evs(
	CombinedStatsFor<generation> combined,
	Species const species,
	Level const level,
	tv::optional<HiddenPower<generation>> const hidden_power,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStatsFor<generation> {
	auto const base_stats = BaseStats(generation, species);
	while (true) {
		auto const previous = combined;
		combined.evs = pad_evs(combined.evs, include_attack, include_special_attack);
		auto const stats = Stats<stat_style_for(generation)>(base_stats, level, combined);
		combined = compute_minimal_spread(base_stats, stats, level, hidden_power, include_attack, include_special_attack);
		if (previous == combined) {
			return combined;
		}
	}
}

} // namespace technicalmachine
