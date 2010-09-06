// Team data structure
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef TEAM_H_
#define TEAM_H_

#include <vector>
#include "pokemon.h"

struct teams {
	std::vector<pokemon> member;				// All Pokemon on the team
	std::vector<pokemon>::iterator active;	// An iterator to the Pokemon that is currently out
	
	pokemon ddfs;		// Information about the attacker at the time of using Doom Desire / Future Sight
	char counter;	// Set to 3 initially, 1 = delayed attack hits at the end of this turn, 0 = not active

	// Number of turns remaining
	char light_screen;
	char lucky_chant;
	char mist;
	char reflect;
	char safeguard;
	char tailwind;

	char wish;		// 2 = Wish will heal next turn, 1 = Wish will heal this turn, 0 = Wish is not in play
	
	// The entry hazards represent those down on this team's side of the field. Using an entry hazard puts them down on the opponent's side of the field
	// Number of layers of Spikes / Toxic Spikes
	char spikes;
	char toxic_spikes;
	bool stealth_rock;

	bool me;				// Is this my team?
};

#endif
