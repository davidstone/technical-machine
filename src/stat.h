// Stats data structures
// Copyright (C) 2011 David Stone
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

#ifndef STAT_H_
#define STAT_H_

#include <cstdint>
#include <string>

#include "species.h"

namespace technicalmachine {

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
			ACC,
			EVA,
			END_STAT
		};

		unsigned max;		// Max HP only
		unsigned stat;		// Current HP or last calculated value for other stats
		uint8_t base;
		int8_t iv;			// 0 through 31
		int8_t ev;			// 0 through 63
	
		Stat (Species name, Stats stat);
		static void boost (int8_t & stage, int n);
	private:
		static uint8_t get_base_stat (Species name, Stats stat);
};

class Nature {
	public:
		enum Natures {
			HARDY,
			LONELY,
			BRAVE,
			ADAMANT,
			NAUGHTY,
			BOLD,
			DOCILE,
			RELAXED,
			IMPISH,
			LAX,
			TIMID,
			HASTY,
			SERIOUS,
			JOLLY,
			NAIVE,
			MODEST,
			MILD,
			QUIET,
			BASHFUL,
			RASH,
			CALM,
			GENTLE,
			SASSY,
			CAREFUL,
			QUIRKY,
			END_NATURE
		};
		Natures name;
		Nature ();
		bool is_set () const;
		static Natures name_from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
};

class Pokemon;
class Team;
class Weather;

unsigned hitpoints (Pokemon & member);
void calculate_attacking_stat (Team & attacker, Weather const & weather);
void calculate_defending_stat (Team const & attacker, Team & defender, Weather const & weather);

// Needed in header for Download.
void calculate_defense (Team const & attacker, Team & defender, Weather const & weather);
void calculate_special_defense (Team const & attacker, Team & defender, Weather const & weather);

void calculate_speed (Team & team, Weather const & weather);
void order (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void faster_pokemon (Team & team1, Team & team2, Weather const & weather, Team* & faster, Team* & slower);
void chance_to_hit (Team & user, Team const & target, Weather const & weather);

}
#endif		// STAT_H_
