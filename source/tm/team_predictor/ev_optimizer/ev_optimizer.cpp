// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <vector>

#include <bounded/assert.hpp>

export module tm.team_predictor.ev_optimizer.ev_optimizer;

import tm.team_predictor.ev_optimizer.defensive;
import tm.team_predictor.ev_optimizer.offensive;
import tm.team_predictor.ev_optimizer.possible_optimized_ivs;
import tm.team_predictor.ev_optimizer.speed;

import tm.move.category;

import tm.pokemon.hidden_power;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.base_stats;
import tm.stat.calculate;
import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.possible_dvs_or_ivs;
import tm.stat.stat_names;
import tm.stat.stats;

import tm.generation;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

template<Generation generation>
constexpr auto combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats<generation> {
	auto best = tv::optional<CombinedStats<generation>>();
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == containers::end(d)) {
			continue;
		}
		auto candidate = CombinedStats<generation>{
			speed.nature,
			IVs(
				defensive->hp.iv,
				offensive->attack.iv,
				defensive->defense.iv,
				offensive->special_attack.iv,
				defensive->special_defense.iv,
				speed.iv
			),
			EVs(
				defensive->hp.ev,
				offensive->attack.ev,
				defensive->defense.ev,
				offensive->special_attack.ev,
				defensive->special_defense.ev,
				speed.ev
			)
		};
		if (!best or ev_sum(candidate.evs) < ev_sum(best->evs)) {
			insert(best, candidate);
		}
	}
	BOUNDED_ASSERT(best);
	BOUNDED_ASSERT(ev_sum(best->evs) <= max_total_evs(generation));
	return *best;
}

export template<Generation generation>
constexpr auto compute_minimal_spread(
	BaseStats const base_stats,
	Stats<generation> stats,
	Level const level,
	tv::optional<HiddenPower<generation>> const hidden_power,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStats<generation> {
	auto const dvs_or_ivs = possible_dvs_or_ivs(hidden_power);

	if constexpr (generation <= Generation::two) {
		return CombinedStats<generation>{
			Nature::Hardy,
			DVs(
				include_attack ? containers::back(dvs_or_ivs.atk()) : containers::front(dvs_or_ivs.atk()),
				containers::back(dvs_or_ivs.def()),
				containers::back(dvs_or_ivs.spe()),
				containers::back(dvs_or_ivs.spc())
			),
			OldGenEVs(
				EV(EV::useful_max),
				include_attack ? EV(EV::useful_max) : EV(0_bi),
				EV(EV::useful_max),
				EV(EV::useful_max),
				EV(EV::useful_max)
			)
		};
	} else {
		return combine<generation>(
			OffensiveEVs(
				level,
				OffensiveEVAtk<generation>{base_stats.atk(), include_attack ? possible_optimized_ivs(dvs_or_ivs.atk()) : possible_optimized_ivs(containers::reversed(dvs_or_ivs.atk())), stats.atk(), include_attack},
				OffensiveEVSpA<generation>{base_stats.spa(), possible_optimized_ivs(dvs_or_ivs.spa()), stats.spa(), include_special_attack}
			),
			DefensiveEVs(
				level,
				DefensiveEVHP{base_stats.hp(), possible_optimized_ivs(dvs_or_ivs.hp()), stats.hp().max()},
				DefensiveEVDef<generation>{base_stats.def(), possible_optimized_ivs(dvs_or_ivs.def()), stats.def()},
				DefensiveEVSpD<generation>{base_stats.spd(), possible_optimized_ivs(dvs_or_ivs.spd()), stats.spd()}
			),
			SpeedEVs(base_stats.spe(), level, possible_optimized_ivs(dvs_or_ivs.spe()), SpeedEVs::Input<generation>{stats.spe()})
		);
	}
}

export template<Generation generation>
auto pad_random_evs(CombinedStats<generation> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<generation> {
	if constexpr (generation <= Generation::two) {
		for (auto const stat_name : containers::enum_range<SpecialPermanentStat>()) {
			auto const minimize_stat = stat_name == SpecialPermanentStat::atk and !include_attack;
			combined.evs[stat_name] = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
		}
	} else {
		auto distribution = std::discrete_distribution{};
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
