// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/move/category.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/level.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/base_stats.hpp>
#include <tm/stat/calculate.hpp>
#include <tm/stat/combined_stats.hpp>
#include <tm/stat/ev.hpp>
#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>
#include <tm/stat/hp.hpp>
#include <tm/stat/iv.hpp>
#include <tm/stat/stat_to_ev.hpp>
#include <tm/type/type.hpp>

#include <bounded/assert.hpp>
#include <bounded/optional.hpp>

#include <containers/begin_end.hpp>

#include <random>

namespace technicalmachine {
using namespace bounded::literal;

namespace detail {

template<Generation generation>
auto combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats<generation> {
	auto best = bounded::optional<CombinedStats<generation>>{};
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
				speed.stat.iv
			),
			EVs(
				defensive->hp.ev,
				offensive->attack.ev,
				defensive->defense.ev,
				offensive->special_attack.ev,
				defensive->special_defense.ev,
				speed.stat.ev
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

} // namespace detail

template<Generation generation>
auto compute_minimal_spread(
	BaseStats const base_stats,
	Stats<generation> stats,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStats<generation> {
	auto const ivs = hidden_power_ivs(generation, hidden_power_type, include_attack);

	if constexpr (generation <= Generation::two) {
		return CombinedStats<generation>{
			Nature::Hardy,
			ivs,
			EVs(
				EV(EV::useful_max),
				include_attack ? EV(EV::useful_max) : EV(0_bi),
				EV(EV::useful_max),
				EV(EV::useful_max),
				EV(EV::useful_max),
				EV(EV::useful_max)
			)
		};
	} else {
		return detail::combine<generation>(
			OffensiveEVs(
				base_stats,
				level,
				OffensiveEVInputs<generation>{ivs.atk(), stats.atk(), include_attack},
				OffensiveEVInputs<generation>{ivs.spa(), stats.spa(), include_special_attack}
			),
			DefensiveEVs(
				base_stats,
				level,
				DefensiveEVs::InputHP{ivs.hp(), stats.hp().max()},
				DefensiveEVs::InputStat<generation>{ivs.def(), stats.def()},
				DefensiveEVs::InputStat<generation>{ivs.spd(), stats.spd()}
			),
			SpeedEVs(base_stats, level, ivs.spe(), SpeedEVs::Input<generation>{stats.spe()})
		);
	}
}

template<Generation generation>
auto pad_random_evs(CombinedStats<generation> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<generation> {
	if constexpr (generation <= Generation::two) {
		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			auto const minimize_stat = stat_name == PermanentStat::atk and !include_attack;
			combined.evs[stat_name] = minimize_stat ? EV(0_bi) : EV(EV::useful_max);
		}
	} else {
		auto distribution = std::discrete_distribution{};
		constexpr auto minimal_increment = 4_bi;
		while (ev_sum(combined.evs) + minimal_increment <= max_total_evs(generation)) {
			distribution.param({
				combined.evs.hp() == EV::useful_max ? 0.0 : 1.0,
				(!include_attack or combined.evs.atk() == EV::useful_max) ? 0.0 : 1.0,
				combined.evs.def() == EV::useful_max ? 0.0 : 1.0,
				(!include_special_attack or combined.evs.spa() == EV::useful_max) ? 0.0 : 1.0,
				combined.evs.spd() == EV::useful_max ? 0.0 : 1.0,
				combined.evs.spe() == EV::useful_max ? 0.0 : 1.0,
			});
			auto const index = distribution(random_engine);
			auto & ev = combined.evs[PermanentStat(index - 1)];
			ev = EV(EV::value_type(ev.value() + minimal_increment));
		}
	}
	return combined;
}

template<Generation generation>
auto optimize_evs(
	CombinedStats<generation> combined,
	Species const species,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack,
	std::mt19937 & random_engine
) -> CombinedStats<generation> {
	auto const base_stats = BaseStats(generation, species);
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(combined, include_attack, include_special_attack, random_engine);
		auto const stats = Stats<generation>(base_stats, level, combined);
		combined = compute_minimal_spread(base_stats, stats, level, hidden_power_type, include_attack, include_special_attack);
		// Technically this isn't correct based on how I pad: I could have some
		// leftover EVs that could have done some good somewhere else, but were
		// not enough to increase the stat they were randomly assigned to.
		if (previous == combined) {
			return combined;
		}
	}
}

template<Generation generation>
void optimize_evs(Pokemon<generation> & pokemon, std::mt19937 & random_engine) {
	auto const species = pokemon.species();
	auto const level = pokemon.level();
	auto const include_attack = has_physical_move(pokemon);
	auto const include_special_attack = has_special_move(pokemon);
	auto const optimized = optimize_evs<generation>(
		calculate_ivs_and_evs(pokemon),
		species,
		level,
		get_hidden_power_type(pokemon),
		include_attack,
		include_special_attack,
		random_engine
	);
	pokemon.set_ivs_and_evs(optimized);
}

} // namespace technicalmachine
