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
#include <tm/pokemon/pokemon.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/all_any_none.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto set_stats(Generation const generation, Pokemon & pokemon, CombinedStats const stats) {
	set_nature(pokemon, stats.nature);

	auto const original_hp = get_hp(pokemon);
	set_hp_ev(generation, pokemon, stats.hp);
	auto const new_hp = get_hp(pokemon);
	pokemon.set_hp(new_hp.max() * original_hp.current() / original_hp.max());

	set_stat_ev(pokemon, StatNames::ATK, stats.attack);
	set_stat_ev(pokemon, StatNames::DEF, stats.defense);
	set_stat_ev(pokemon, StatNames::SPA, stats.special_attack);
	set_stat_ev(pokemon, StatNames::SPD, stats.special_defense);
	set_stat_ev(pokemon, StatNames::SPE, stats.speed);
}

bool has_physical_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), [](Move const move) { return is_physical(move.name()); });
}

bool has_special_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), [](Move const move) { return is_special(move.name()); });
}

auto combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> CombinedStats {
	auto best = bounded::optional<CombinedStats>{};
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == end(d)) {
			continue;
		}
		auto candidate = CombinedStats{
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
	BOUNDED_ASSERT(ev_sum(*best) <= EV::max_total);
	return *best;
}

auto optimize_evs(Generation const generation, CombinedStats combined, Species const species, Level const level, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) {
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(combined, include_attack, include_special_attack, random_engine);
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

auto pull_out_stats(Pokemon const & pokemon) -> CombinedStats {
	return CombinedStats{
		get_nature(pokemon),
		get_hp(pokemon).ev(),
		get_stat(pokemon, StatNames::ATK).ev(),
		get_stat(pokemon, StatNames::DEF).ev(),
		get_stat(pokemon, StatNames::SPA).ev(),
		get_stat(pokemon, StatNames::SPD).ev(),
		get_stat(pokemon, StatNames::SPE).ev()
	};
}

void optimize_evs(Generation const generation, Pokemon & pokemon, std::mt19937 & random_engine) {
	auto const species = get_species(pokemon);
	auto const level = get_level(pokemon);
	auto const include_attack = has_physical_move(pokemon);
	auto const include_special_attack = has_special_move(pokemon);
	auto const optimized = optimize_evs(generation, pull_out_stats(pokemon), species, level, include_attack, include_special_attack, random_engine);
	set_stats(generation, pokemon, optimized);
}

auto minimize_evs(Generation const generation, CombinedStats const stats, Species const species, Level const level, bool const include_attack, bool const include_special_attack) -> CombinedStats {
	auto const nature = stats.nature;
	auto const hp = HP(generation, species, level, stats.hp);
	auto const attack = Stat(generation, species, StatNames::ATK, stats.attack);
	auto const defense = Stat(generation, species, StatNames::DEF, stats.defense);
	auto const special_attack = Stat(generation, species, StatNames::SPA, stats.special_attack);
	auto const special_defense = Stat(generation, species, StatNames::SPD, stats.special_defense);
	auto const speed = Stat(generation, species, StatNames::SPE, stats.speed);

	auto const result = combine(
		OffensiveEVs(generation, species, level, nature, attack, special_attack, include_attack, include_special_attack),
		DefensiveEVs(generation, species, level, nature, hp, defense, special_defense),
		SpeedEVs(nature, speed, level)
	);
	return result;
}

auto pad_random_evs(CombinedStats combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> CombinedStats {
	auto distribution = std::discrete_distribution{};
	while (ev_sum(combined) < EV::max_total) {
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
