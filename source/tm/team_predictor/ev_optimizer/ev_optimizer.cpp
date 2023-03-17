// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.ev_optimizer.ev_optimizer;

import tm.team_predictor.ev_optimizer.compute_minimal_spread;
import tm.team_predictor.ev_optimizer.pad_random_evs;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.combined_stats;
import tm.stat.stats;

import tm.generation;

import tv;
import std_module;

namespace technicalmachine {

export template<Generation generation>
auto optimize_evs(
	CombinedStats<generation> combined,
	Species const species,
	Level const level,
	tv::optional<HiddenPower<generation>> const hidden_power,
	bool const include_attack,
	bool const include_special_attack,
	std::mt19937 & random_engine
) -> CombinedStats<generation> {
	auto const base_stats = BaseStats(generation, species);
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(combined, include_attack, include_special_attack, random_engine);
		auto const stats = Stats<generation>(base_stats, level, combined);
		combined = compute_minimal_spread(base_stats, stats, level, hidden_power, include_attack, include_special_attack);
		// Technically this isn't correct based on how I pad: I could have some
		// leftover EVs that could have done some good somewhere else, but were
		// not enough to increase the stat they were randomly assigned to.
		if (previous == combined) {
			return combined;
		}
	}
}

} // namespace technicalmachine
