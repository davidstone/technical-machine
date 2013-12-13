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
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
class ActivePokemon;
class Level;
class Pokemon;
class Team;
class Weather;

class Stat {
public:
	// I set HP to -1 so it doesn't get in the way of indexing stat boosts in an array.
	enum Stats {
		HP = -1,
		ATK,
		DEF,
		SPA,
		SPD,
		SPE,
		NORMAL_END,
		ACC = NORMAL_END,
		EVA,
		END
	};
	typedef uint16_t stat_type;
	stat_type max;		// Max HP only
	stat_type stat;		// Current HP or last calculated value for other stats
	using base_type = bounded_integer::checked_integer<1, 255>;
	base_type base;
	bounded_integer::checked_integer<0, 31> iv;
	EV ev;

	Stat (Species name, Stats stat);
	void calculate_initial_hp(Level level);
};

inline constexpr std::initializer_list<Stat::Stats> regular_stats() {
	return { Stat::HP, Stat::ATK, Stat::DEF, Stat::SPA, Stat::SPD, Stat::SPE };
}

template<Stat::Stats>
unsigned initial_stat(Pokemon const & pokemon);
extern template unsigned initial_stat<Stat::ATK>(Pokemon const & pokemon);
extern template unsigned initial_stat<Stat::SPA>(Pokemon const & pokemon);
extern template unsigned initial_stat<Stat::DEF>(Pokemon const & pokemon);
extern template unsigned initial_stat<Stat::SPD>(Pokemon const & pokemon);
extern template unsigned initial_stat<Stat::SPE>(Pokemon const & pokemon);


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
