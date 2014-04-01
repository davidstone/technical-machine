// Calculate a Pokemon's current stat
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

#ifndef STAT__CALCULATE_HPP_
#define STAT__CALCULATE_HPP_

#include <bounded_integer/bounded_integer.hpp>

#include "nature.hpp"
#include "stat.hpp"
#include "stat_names.hpp"

#include "../pokemon/level.hpp"

namespace technicalmachine {
using namespace bounded::literal;

class ActivePokemon;
class Team;
class Weather;

template<StatNames stat_name>
auto initial_stat(Stat const & stat, Level const & level, Nature const & nature) {
	auto const pre_nature = (2_bi * stat.base + stat.iv.value() + stat.ev.value() / 4_bi) * level() / 100_bi + 5_bi;
	return pre_nature * nature.boost<stat_name>();
}

using attack_type = bounded::integer<1, 7368>;
using special_attack_type = bounded::integer<1, 4536>;
std::common_type<attack_type, special_attack_type>::type calculate_attacking_stat(ActivePokemon const & attacker, Weather const & weather);
attack_type calculate_attack(ActivePokemon const & attacker, Weather const & weather);
special_attack_type calculate_special_attack(ActivePokemon const & attacker, Weather const & weather);

using defense_type = bounded::integer<1, 3684>;
using special_defense_type = bounded::integer<1, 3684>;
std::common_type<defense_type, special_defense_type>::type calculate_defending_stat(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather);
defense_type calculate_defense(ActivePokemon const & defender, Weather const & weather, bool ch = false, bool is_self_KO = false);
special_defense_type calculate_special_defense(ActivePokemon const & defender, Weather const & weather, bool ch = false);

using speed_type = bounded::integer<1, 12096>;
speed_type calculate_speed(Team const & team, Weather const & weather);
void order(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void faster_pokemon(Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);

}	// namespace technicalmachine
#endif	// STAT__CALCULATE_HPP_
