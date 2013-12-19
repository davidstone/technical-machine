// Stats data structures
// Copyright (C) 2013 David Stone
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
#include <initializer_list>
#include <bounded_integer/bounded_integer.hpp>
#include "ev.hpp"
#include "iv.hpp"
#include "nature.hpp"
#include "stat_names.hpp"
#include "../pokemon/level.hpp"
#include "../pokemon/species_forward.hpp"
#include "../rational.hpp"

namespace technicalmachine {
class ActivePokemon;
class Pokemon;
class Team;
class Weather;

class Stat {
public:
	typedef uint16_t stat_type;
	bounded_integer::checked_integer<1, 714> max;		// Max HP only
	stat_type stat;		// Current HP or last calculated value for other stats
	using base_type = bounded_integer::checked_integer<1, 255>;
	base_type base;
	IV iv;
	EV ev;

	Stat (Species name, StatNames stat);
	void calculate_initial_hp(Level level);
};

inline constexpr std::initializer_list<StatNames> regular_stats() {
	return { StatNames::HP, StatNames::ATK, StatNames::DEF, StatNames::SPA, StatNames::SPD, StatNames::SPE };
}

namespace detail {

inline auto initial_generic_stat(Stat const & stat, Level const level) {
	return (2_bi * stat.base + stat.iv.value() + stat.ev.value() / 4_bi) * level() / 100_bi + 5_bi;
}

}	// namespace detail

template<StatNames stat_name>
unsigned initial_stat(Stat const & stat, Level const & level, Nature const & nature) {
	return static_cast<unsigned>(detail::initial_generic_stat(stat, level)) * nature.boost<stat_name>();
}

inline auto initial_hp(Stat const & hp, Level const & level) {
	return bounded_integer::ternary_conditional((hp.base > 1_bi), (detail::initial_generic_stat(hp, level) + level() + 5_bi), 1_bi);
}


void calculate_attacking_stat (ActivePokemon & attacker, Weather const & weather);
void calculate_attack(ActivePokemon & attacker, Weather const & weather);
void calculate_special_attack(ActivePokemon & attacker, Weather const & weather);

void calculate_defending_stat (ActivePokemon const & attacker, ActivePokemon & defender, Weather const & weather);
void calculate_defense (ActivePokemon & defender, Weather const & weather, bool ch = false, bool is_self_KO = false);
void calculate_special_defense (ActivePokemon & defender, Weather const & weather, bool ch = false);

void calculate_speed (Team & team, Weather const & weather);
void order (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void faster_pokemon (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);

}	// namespace technicalmachine
#endif	// STAT__STAT_HPP_
