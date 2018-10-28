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

#include <tm/pokemon/pokemon.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto ev_sum(Combined const stats) {
	constexpr auto impl = [](auto... args) { return (... + args.value()); };
	return impl(stats.hp, stats.attack, stats.defense, stats.special_attack, stats.special_defense, stats.speed);
}

auto set_stats(Pokemon & pokemon, Combined const stats) {
	get_nature(pokemon) = stats.nature;
	set_hp_ev(pokemon, stats.hp);
	set_stat_ev(pokemon, StatNames::ATK, stats.attack);
	set_stat_ev(pokemon, StatNames::DEF, stats.defense);
	set_stat_ev(pokemon, StatNames::SPA, stats.special_attack);
	set_stat_ev(pokemon, StatNames::SPD, stats.special_defense);
	set_stat_ev(pokemon, StatNames::SPE, stats.speed);
}

bool has_physical_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_physical);
}

bool has_special_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_special);
}

auto combine(OffensiveEVs const & o, DefensiveEVs const & d, SpeedEVs const & speed_container) -> Combined {
	auto best = bounded::optional<Combined>{};
	for (auto const & speed : speed_container) {
		auto const offensive = o.find(speed.nature);
		if (!offensive) {
			continue;
		}
		auto const defensive = d.find(speed.nature);
		if (defensive == end(d)) {
			continue;
		}
		auto candidate = Combined{
			speed.nature,
			defensive->hp,
			offensive->attack,
			defensive->defense,
			offensive->special_attack,
			defensive->special_defense,
			speed.ev
		};
		if (!best or ev_sum(candidate) < ev_sum(*best)) {
			best.emplace(candidate);
		}
	}
	assert(best);
	assert(ev_sum(*best) <= EV::max_total);
	return *best;
}

auto optimize_evs(Combined combined, Species const species, Level const level, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) {
	while (true) {
		auto const previous = combined;
		combined = pad_random_evs(combined, include_attack, include_special_attack, random_engine);
		combined = minimize_evs(combined, species, level, include_attack, include_special_attack);
		// Technically this isn't correct based on how I pad: I could have some
		// leftover EVs that could have done some good somewhere else, but were
		// not enough to increase the stat they were randomly assigned to.
		if (previous == combined) {
			return combined;
		}
	};
}

}	// namespace

auto pull_out_stats(Pokemon const & pokemon) -> Combined {
	return Combined{
		get_nature(pokemon),
		get_hp(pokemon).ev(),
		get_stat(pokemon, StatNames::ATK).ev(),
		get_stat(pokemon, StatNames::DEF).ev(),
		get_stat(pokemon, StatNames::SPA).ev(),
		get_stat(pokemon, StatNames::SPD).ev(),
		get_stat(pokemon, StatNames::SPE).ev()
	};
}

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	auto const species = get_species(pokemon);
	auto const level = get_level(pokemon);
	auto const include_attack = has_physical_move(pokemon);
	auto const include_special_attack = has_special_move(pokemon);
	auto const optimized = optimize_evs(pull_out_stats(pokemon), species, level, include_attack, include_special_attack, random_engine);
	set_stats(pokemon, optimized);
}

auto minimize_evs(Combined const stats, Species const species, Level const level, bool const include_attack, bool const include_special_attack) -> Combined {
	auto const nature = stats.nature;
	auto const hp = HP(species, level, stats.hp);
	auto const attack = Stat(species, StatNames::ATK, stats.attack);
	auto const defense = Stat(species, StatNames::DEF, stats.defense);
	auto const special_attack = Stat(species, StatNames::SPA, stats.special_attack);
	auto const special_defense = Stat(species, StatNames::SPD, stats.special_defense);
	auto const speed = Stat(species, StatNames::SPE, stats.speed);

	auto const result = combine(
		OffensiveEVs(species, level, nature, attack, special_attack, include_attack, include_special_attack),
		DefensiveEVs(species, level, nature, hp, defense, special_defense),
		SpeedEVs(nature, speed, level)
	);
	return result;
}

auto pad_random_evs(Combined combined, bool const include_attack, bool const include_special_attack, std::mt19937 & random_engine) -> Combined {
	auto distribution = std::discrete_distribution{};
	while (ev_sum(combined) < EV::max_total) {
		distribution.param({
			combined.hp.value() == EV::max ? 0.0 : 1.0,
			(!include_attack or combined.attack.value() == EV::max) ? 0.0 : 1.0,
			combined.defense.value() == EV::max ? 0.0 : 1.0,
			(!include_special_attack or combined.special_attack.value() == EV::max) ? 0.0 : 1.0,
			combined.special_defense.value() == EV::max ? 0.0 : 1.0,
			combined.speed.value() == EV::max ? 0.0 : 1.0,
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
