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

auto set_stats(Generation const generation, Pokemon & pokemon, CombinedStats<EV> const stats) {
	set_nature(pokemon, stats.nature);

	auto const original_hp = get_hp(pokemon);
	set_hp_ev(generation, pokemon, original_hp.iv(), stats.hp);
	auto const new_hp = get_hp(pokemon);
	pokemon.set_hp(new_hp.max() * original_hp.current() / original_hp.max());

	auto set = [&](StatNames const name, EV const ev) {
		set_stat_ev(pokemon, name, get_stat(pokemon, name).iv(), ev);
	};

	set(StatNames::ATK, stats.attack);
	set(StatNames::DEF, stats.defense);
	set(StatNames::SPA, stats.special_attack);
	set(StatNames::SPD, stats.special_defense);
	set(StatNames::SPE, stats.speed);
}

auto combine(Generation const generation, OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats<EV> {
	auto best = bounded::optional<CombinedStats<EV>>{};
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == end(d)) {
			continue;
		}
		auto candidate = CombinedStats<EV>{
			speed.nature,
			defensive->hp,
			offensive->attack,
			defensive->defense,
			offensive->special_attack,
			defensive->special_defense,
			speed.ev
		};
		if (!best or ev_sum(candidate) < ev_sum(*best)) {
			insert(best, candidate);
		}
	}
	BOUNDED_ASSERT(best);
	BOUNDED_ASSERT(ev_sum(*best) <= max_total_evs(generation));
	return *best;
}

auto optimize_evs(Generation const generation, CombinedStats<EV> combined, Species const species, Level const level, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) {
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
	auto const optimized = optimize_evs(generation, calculate_evs(generation, pokemon), species, level, include_attack, include_special_attack, random_engine);
	set_stats(generation, pokemon, optimized);
}

auto minimize_evs(Generation const generation, CombinedStats<EV> const stats, Species const species, Level const level, bool const include_attack, bool const include_special_attack) -> CombinedStats<EV> {
	if (generation <= Generation::two) {
		return CombinedStats<EV>{
			stats.nature,
			stats.hp,
			include_attack ? stats.attack : EV(0_bi),
			stats.defense,
			stats.special_attack,
			stats.special_defense,
			stats.speed
		};
	}
	auto const iv = default_iv(generation);
	auto const nature = stats.nature;
	auto const hp = HP(generation, species, level, iv, stats.hp);
	auto const attack = Stat(generation, species, StatNames::ATK, iv, stats.attack);
	auto const defense = Stat(generation, species, StatNames::DEF, iv, stats.defense);
	auto const special_attack = Stat(generation, species, StatNames::SPA, iv, stats.special_attack);
	auto const special_defense = Stat(generation, species, StatNames::SPD, iv, stats.special_defense);
	auto const speed = Stat(generation, species, StatNames::SPE, iv, stats.speed);

	return combine(
		generation,
		OffensiveEVs(generation, species, level, nature, attack, special_attack, include_attack, include_special_attack),
		DefensiveEVs(generation, species, level, nature, hp, defense, special_defense),
		SpeedEVs(nature, speed, level)
	);
}

auto pad_random_evs(Generation const generation, CombinedStats<EV> combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats<EV> {
	if (generation <= Generation::two) {
		combined.hp = EV(EV::max);
		combined.attack = include_attack ? EV(EV::max) : EV(0_bi);
		combined.defense = EV(EV::max);
		combined.special_attack = EV(EV::max);
		combined.special_defense = EV(EV::max);
		combined.speed = EV(EV::max);
		return combined;
	}
	auto distribution = std::discrete_distribution{};
	while (ev_sum(combined) < max_total_evs(generation)) {
		distribution.param({
			combined.hp == EV::max ? 0.0 : 1.0,
			(!include_attack or combined.attack == EV::max) ? 0.0 : 1.0,
			combined.defense == EV::max ? 0.0 : 1.0,
			(!include_special_attack or combined.special_attack == EV::max) ? 0.0 : 1.0,
			combined.special_defense == EV::max ? 0.0 : 1.0,
			combined.speed == EV::max ? 0.0 : 1.0,
		});
		auto const index = distribution(random_engine);
		auto & ev =
			index == 0 ? combined.hp :
			index == 1 ? combined.attack :
			index == 2 ? combined.defense :
			index == 3 ? combined.special_attack :
			index == 4 ? combined.special_defense :
			combined.speed;
		ev = EV(EV::value_type(ev.value() + 4_bi));
	}
	return combined;
}

}	// namespace technicalmachine
