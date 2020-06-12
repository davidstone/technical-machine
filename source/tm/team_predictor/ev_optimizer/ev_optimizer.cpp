// Optimize EVs and nature to remove waste
// Copyright (C) 2018 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>

#include <tm/team_predictor/ev_optimizer/defensive.hpp>
#include <tm/team_predictor/ev_optimizer/offensive.hpp>
#include <tm/team_predictor/ev_optimizer/speed.hpp>

#include <tm/move/category.hpp>

#include <tm/pokemon/has_physical_or_special_move.hpp>
#include <tm/pokemon/pokemon.hpp>

#include <tm/stat/generic_stats.hpp>
#include <tm/stat/hidden_power_ivs.hpp>
#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto set_stats(Generation const generation, Pokemon & pokemon, CombinedStats<IVAndEV> const stats) {
	auto const original_hp = pokemon.hp();
	pokemon.set_nature(stats.nature);
	for (auto const stat_name : containers::enum_range<PermanentStat>()) {
		auto const stat = stats[stat_name];
		pokemon.set_ev(generation, stat_name, stat.iv, stat.ev);
	}
	auto const new_hp = pokemon.hp();
	pokemon.set_hp(new_hp.max() * original_hp.current() / original_hp.max());
}

auto combine(Generation const generation, OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats<IVAndEV> {
	auto best = bounded::optional<CombinedStats<IVAndEV>>{};
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == end(d)) {
			continue;
		}
		auto candidate = CombinedStats<IVAndEV>{
			speed.nature,
			defensive->hp,
			offensive->attack,
			defensive->defense,
			offensive->special_attack,
			defensive->special_defense,
			speed.stat
		};
		if (!best or ev_sum(candidate) < ev_sum(*best)) {
			insert(best, candidate);
		}
	}
	BOUNDED_ASSERT(best);
	BOUNDED_ASSERT(ev_sum(*best) <= max_total_evs(generation));
	return *best;
}

auto optimize_evs(
	Generation const generation,
	CombinedStats<IVAndEV> combined,
	Species const species,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack,
	std::mt19937 & random_engine
) {
	auto const base_stats = BaseStats(generation, species);
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(generation, combined, include_attack, include_special_attack, random_engine);
		auto const stats = initial_stats(base_stats, level, combined);
		combined = compute_minimal_spread(generation, base_stats, stats, level, hidden_power_type, include_attack, include_special_attack);
		// Technically this isn't correct based on how I pad: I could have some
		// leftover EVs that could have done some good somewhere else, but were
		// not enough to increase the stat they were randomly assigned to.
		if (previous == combined) {
			return combined;
		}
	}
}

}	// namespace

void optimize_evs(Generation const generation, Pokemon & pokemon, std::mt19937 & random_engine) {
	auto const species = pokemon.species();
	auto const level = pokemon.level();
	auto const include_attack = has_physical_move(generation, pokemon);
	auto const include_special_attack = has_special_move(generation, pokemon);
	auto const optimized = optimize_evs(
		generation,
		calculate_ivs_and_evs(generation, pokemon),
		species,
		level,
		get_hidden_power_type(pokemon),
		include_attack,
		include_special_attack,
		random_engine
	);
	set_stats(generation, pokemon, optimized);
}

auto compute_minimal_spread(
	Generation const generation,
	BaseStats const base_stats,
	GenericStats<HP::max_type, InitialStat> stats,
	Level const level,
	bounded::optional<Type> const hidden_power_type,
	bool const include_attack,
	bool const include_special_attack
) -> CombinedStats<IVAndEV> {
	auto const ivs = hidden_power_ivs(generation, hidden_power_type, include_attack);

	if (generation <= Generation::two) {
		return {
			Nature::Hardy,
			{ivs.hp, EV(EV::max)},
			{ivs.atk, include_attack ? EV(EV::max) : EV(0_bi)},
			{ivs.def, EV(EV::max)},
			{ivs.spa, EV(EV::max)},
			{ivs.spd, EV(EV::max)},
			{ivs.spe, EV(EV::max)},
		};
	}

	return combine(
		generation,
		OffensiveEVs(
			base_stats,
			level,
			OffensiveEVs::Input{ivs.atk, stats.atk, include_attack},
			OffensiveEVs::Input{ivs.spa, stats.spa, include_special_attack}
		),
		DefensiveEVs(
			base_stats,
			level,
			DefensiveEVs::InputHP{ivs.hp, stats.hp},
			DefensiveEVs::InputStat{ivs.def, stats.def},
			DefensiveEVs::InputStat{ivs.spd, stats.spd}
		),
		SpeedEVs(base_stats, level, ivs.spe, stats.spe)
	);
}

auto pad_random_evs(Generation const generation, CombinedStats<IVAndEV> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<IVAndEV> {
	if (generation <= Generation::two) {
		for (auto const stat_name : containers::enum_range<PermanentStat>()) {
			auto const minimize_stat = stat_name == PermanentStat::atk and !include_attack;
			combined[stat_name].ev = minimize_stat ? EV(0_bi) : EV(EV::max);
		}
		return combined;
	}
	auto distribution = std::discrete_distribution{};
	while (ev_sum(combined) < max_total_evs(generation)) {
		distribution.param({
			combined.hp.ev == EV::max ? 0.0 : 1.0,
			(!include_attack or combined.atk.ev == EV::max) ? 0.0 : 1.0,
			combined.def.ev == EV::max ? 0.0 : 1.0,
			(!include_special_attack or combined.spa.ev == EV::max) ? 0.0 : 1.0,
			combined.spd.ev == EV::max ? 0.0 : 1.0,
			combined.spe.ev == EV::max ? 0.0 : 1.0,
		});
		auto const index = distribution(random_engine);
		auto & ev = combined[PermanentStat(index - 1)].ev;
		ev = EV(EV::value_type(ev.value() + 4_bi));
	}
	return combined;
}

}	// namespace technicalmachine
