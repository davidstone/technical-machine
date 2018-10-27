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

#include "ev_optimizer.hpp"

#include "combine.hpp"
#include "defensive.hpp"
#include "offensive.hpp"
#include "speed.hpp"

#include "../../pokemon/pokemon.hpp"

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/transform.hpp>

#include <random>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto ev_sum(Pokemon const & pokemon) {
	auto const ev_value = [&](StatNames const stat) {
		return get_stat(pokemon, stat).ev().value();
	};
	return EV::total_type(containers::accumulate(
		containers::transform(regular_stats(), ev_value),
		get_hp(pokemon).ev().value()
	));
}

bool has_physical_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_physical);
}

bool has_special_move(Pokemon const & pokemon) {
	return containers::any(regular_moves(pokemon), is_special);
}

}	// namespace

void optimize_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	minimize_evs(pokemon);
	while (ev_sum(pokemon) < EV::max_total) {
		pad_random_evs(pokemon, random_engine);
		minimize_evs(pokemon);
	}
}

void minimize_evs(Pokemon & pokemon) {
	auto const species = static_cast<Species>(pokemon);
	auto const level = get_level(pokemon);
	auto const nature = get_nature(pokemon);
	auto const hp = get_hp(pokemon);
	auto stat = [&](StatNames const name) { return get_stat(pokemon, name); };

	auto const offensive = OffensiveEVs(species, level, nature, stat(StatNames::ATK), stat(StatNames::SPA), has_physical_move(pokemon), has_special_move(pokemon));
	auto const defensive = DefensiveEVs(species, level, nature, hp, stat(StatNames::DEF), stat(StatNames::SPD));
	auto const speed = SpeedEVs(nature, stat(StatNames::SPE), level);

	auto const result = combine(offensive, defensive, speed);
	set_hp_ev(pokemon, result.hp);
	set_stat_ev(pokemon, StatNames::ATK, result.attack);
	set_stat_ev(pokemon, StatNames::DEF, result.defense);
	set_stat_ev(pokemon, StatNames::SPA, result.special_attack);
	set_stat_ev(pokemon, StatNames::SPD, result.special_defense);
	set_stat_ev(pokemon, StatNames::SPE, result.speed);
	get_nature(pokemon) = result.nature;
}

void pad_random_evs(Pokemon & pokemon, std::mt19937 & random_engine) {
	auto distribution = std::discrete_distribution{};
	while (ev_sum(pokemon) < EV::max_total) {
		auto remaining_evs = [&](auto const stat_name) {
			auto const full_at_zero =
				(stat_name == StatNames::ATK and !has_physical_move(pokemon)) or
				(stat_name == StatNames::SPA and !has_special_move(pokemon));
			auto const full = full_at_zero or get_stat(pokemon, stat_name).ev().value() == EV::max;
			return full ? 0.0 : 1.0;
		};
		distribution.param({
			get_hp(pokemon).ev().value() == EV::max ? 0.0 : 1.0,
			remaining_evs(StatNames::ATK),
			remaining_evs(StatNames::DEF),
			remaining_evs(StatNames::SPA),
			remaining_evs(StatNames::SPD),
			remaining_evs(StatNames::SPE),
		});
		auto const index = distribution(random_engine);
		if (index == 0) {
			set_hp_ev(pokemon, EV(EV::value_type(get_hp(pokemon).ev().value() + 4_bi)));
		} else {
			auto const stat_name = regular_stats()[bounded::integer<0, 4>(index - 1)];
			set_stat_ev(pokemon, stat_name, EV(EV::value_type(get_stat(pokemon, stat_name).ev().value() + 4_bi)));
		}
	}
}

}	// namespace technicalmachine
