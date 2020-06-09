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

#include <tm/stat/stat_to_ev.hpp>

#include <bounded/assert.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto set_stats(Generation const generation, Pokemon & pokemon, CombinedStats<IVAndEV> const stats) {
	set_nature(pokemon, stats.nature);

	auto const original_hp = get_hp(pokemon);
	set_hp_ev(generation, pokemon, stats.hp.iv, stats.hp.ev);
	auto const new_hp = get_hp(pokemon);
	pokemon.set_hp(new_hp.max() * original_hp.current() / original_hp.max());

	auto set = [&](RegularStat const stat_name, IVAndEV const stat) {
		set_stat_ev(pokemon, stat_name, stat.iv, stat.ev);
	};

	set(RegularStat::atk, stats.attack);
	set(RegularStat::def, stats.defense);
	set(RegularStat::spa, stats.special_attack);
	set(RegularStat::spd, stats.special_defense);
	set(RegularStat::spe, stats.speed);
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

auto optimize_evs(Generation const generation, CombinedStats<IVAndEV> combined, Species const species, Level const level, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) {
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(generation, combined, include_attack, include_special_attack, random_engine);
		combined = minimize_evs(generation, combined, species, level, include_attack, include_special_attack);
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
	auto const species = get_species(pokemon);
	auto const level = get_level(pokemon);
	auto const include_attack = has_physical_move(generation, pokemon);
	auto const include_special_attack = has_special_move(generation, pokemon);
	auto const optimized = optimize_evs(generation, calculate_ivs_and_evs(generation, pokemon), species, level, include_attack, include_special_attack, random_engine);
	set_stats(generation, pokemon, optimized);
}

auto minimize_evs(Generation const generation, CombinedStats<IVAndEV> const stats, Species const species, Level const level, bool const include_attack, bool const include_special_attack) -> CombinedStats<IVAndEV> {
	if (generation <= Generation::two) {
		return CombinedStats<IVAndEV>{
			stats.nature,
			stats.hp,
			include_attack ? stats.attack : IVAndEV{stats.attack.iv, EV(0_bi)},
			stats.defense,
			stats.special_attack,
			stats.special_defense,
			stats.speed
		};
	}
	auto const base_stats = BaseStats(generation, species);
	auto const nature = stats.nature;
	auto const hp = HP(base_stats, level, stats.hp.iv, stats.hp.ev).max();
	auto const attack = initial_stat(RegularStat::atk, base_stats.atk(), stats.attack.iv, stats.attack.ev, level, nature);
	auto const defense = initial_stat(RegularStat::def, base_stats.def(), stats.defense.iv, stats.defense.ev, level, nature);
	auto const special_attack = initial_stat(RegularStat::spa, base_stats.spa(), stats.special_attack.iv, stats.special_attack.ev, level, nature);
	auto const special_defense = initial_stat(RegularStat::spd, base_stats.spd(), stats.special_defense.iv, stats.special_defense.ev, level, nature);
	auto const speed = initial_stat(RegularStat::spe, base_stats.spe(), stats.speed.iv, stats.speed.ev, level, nature);

	return combine(
		generation,
		OffensiveEVs(base_stats, level, OffensiveEVs::Input{stats.attack.iv, attack, include_attack}, OffensiveEVs::Input{stats.special_attack.iv, special_attack, include_special_attack}),
		DefensiveEVs(base_stats, level, DefensiveEVs::InputHP{stats.hp.iv, hp}, DefensiveEVs::InputStat{stats.defense.iv, defense}, DefensiveEVs::InputStat{stats.special_defense.iv, special_defense}),
		SpeedEVs(base_stats, level, stats.speed.iv, speed)
	);
}

auto pad_random_evs(Generation const generation, CombinedStats<IVAndEV> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<IVAndEV> {
	if (generation <= Generation::two) {
		combined.hp.ev = EV(EV::max);
		combined.attack.ev = include_attack ? EV(EV::max) : EV(0_bi);
		combined.defense.ev = EV(EV::max);
		combined.special_attack.ev = EV(EV::max);
		combined.special_defense.ev = EV(EV::max);
		combined.speed.ev = EV(EV::max);
		return combined;
	}
	auto distribution = std::discrete_distribution{};
	while (ev_sum(combined) < max_total_evs(generation)) {
		distribution.param({
			combined.hp.ev == EV::max ? 0.0 : 1.0,
			(!include_attack or combined.attack.ev == EV::max) ? 0.0 : 1.0,
			combined.defense.ev == EV::max ? 0.0 : 1.0,
			(!include_special_attack or combined.special_attack.ev == EV::max) ? 0.0 : 1.0,
			combined.special_defense.ev == EV::max ? 0.0 : 1.0,
			combined.speed.ev == EV::max ? 0.0 : 1.0,
		});
		auto const index = distribution(random_engine);
		auto & ev =
			index == 0 ? combined.hp.ev :
			index == 1 ? combined.attack.ev :
			index == 2 ? combined.defense.ev :
			index == 3 ? combined.special_attack.ev :
			index == 4 ? combined.special_defense.ev :
			combined.speed.ev;
		ev = EV(EV::value_type(ev.value() + 4_bi));
	}
	return combined;
}

}	// namespace technicalmachine
