// Stats data structures
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STAT_H_
#define STAT_H_

#include "species.h"

namespace technicalmachine {

enum stats { HP, ATK, DEF, SPA, SPD, SPE };

class Stat {
	public:
		uint8_t base;
		int8_t iv;			// 0 through 31
		int8_t ev;			// 0 through 63
		int8_t stage;		// -6 though 6
		int max;		// Max HP only
		int stat;		// Current HP or last calculated value for other stats
	
		Stat (species name, int level, stats stat);
		void boost (int n);
	private:
		static uint8_t const base_stat [][6];
};

enum natures { ADAMANT, BASHFUL, BOLD, BRAVE, CALM, CAREFUL, DOCILE, GENTLE, HARDY, HASTY, IMPISH, JOLLY, LAX, LONELY, MILD, MODEST, NAIVE, NAUGHTY, QUIET, QUIRKY, RASH, RELAXED, SASSY, SERIOUS, TIMID, END_NATURE };

class Pokemon;
class Team;
class Weather;

int hitpoints (Pokemon &member);
void attack (Team &attacker, Weather const &weather);
void defense (Team const &attacker, Team &defender, Weather const &weather);
void speed (Team &team, Weather const &weather);
void order (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower);
void faster_pokemon (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower);
void chance_to_hit (Team &user, Team const &target, Weather const &weather);

}
#endif
