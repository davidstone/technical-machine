// Stats data structures
// Copyright (C) 2014 David Stone
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

#ifndef STAT__STAT_HPP_
#define STAT__STAT_HPP_

#include <cstdint>
#include <bounded_integer/bounded_integer.hpp>
#include "ev.hpp"
#include "iv.hpp"
#include "nature.hpp"
#include "stat_names.hpp"
#include "../pokemon/level.hpp"
#include "../pokemon/species_forward.hpp"
#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded_integer::literal;
class ActivePokemon;
class Pokemon;
class Team;
class Weather;

class Stat {
public:
	using base_type = bounded_integer::checked_integer<5, 230>;

	Stat(Species name, StatNames stat, EV ev = EV(0_bi));

	base_type base;
	EV ev;
	IV iv;
};

template<StatNames stat_name>
auto initial_stat(Stat const & stat, Level const & level, Nature const & nature) {
	auto const pre_nature = (2_bi * stat.base + stat.iv.value() + stat.ev.value() / 4_bi) * level() / 100_bi + 5_bi;
	return pre_nature * nature.boost<stat_name>();
}

unsigned calculate_attacking_stat(ActivePokemon const & attacker, Weather const & weather);
unsigned calculate_attack(ActivePokemon const & attacker, Weather const & weather);
unsigned calculate_special_attack(ActivePokemon const & attacker, Weather const & weather);

unsigned calculate_defending_stat(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather);
unsigned calculate_defense(ActivePokemon const & defender, Weather const & weather, bool ch = false, bool is_self_KO = false);
unsigned calculate_special_defense(ActivePokemon const & defender, Weather const & weather, bool ch = false);

unsigned calculate_speed(Team const & team, Weather const & weather);
void order(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void faster_pokemon(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);

}	// namespace technicalmachine
#endif	// STAT__STAT_HPP_
