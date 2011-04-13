// Team data structure
// Copyright 2011 David Stone
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

#include <fstream>
#include <string>
#include <vector>
#include "active.h"
#include "pokemon.h"

namespace technicalmachine {

struct Team {
	std::string player;						// The player's name
	std::vector<Pokemon> member;			// All Pokemon on the team
	Active<Pokemon> active;
	
//	Pokemon ddfs;		// Information about the attacker at the time of using Doom Desire / Future Sight
	char counter;			// Set to 3 initially, 1 = delayed attack hits at the end of this turn, 0 = not active

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

	unsigned char replacement;		// If a Pokemon faints, what Pokemon should replace it?
	
	const bool me;				// Is this my team?
	
	bool operator== (const Team &other) const {
		if (this->member.size() != other.member.size())
			return false;
		for (size_t n = 0; n != this->member.size(); ++n) {
			if (this->member.at (n) != other.member.at (n))
				return false;
		}
		return /*this->active->name == other.active->name and */this->counter == other.counter and this->light_screen == other.light_screen and this->lucky_chant == other.lucky_chant and this->mist == other.mist and this->reflect == other.reflect and this->safeguard == other.safeguard and this->tailwind == other.tailwind and this->wish == other.wish and this->spikes == other.spikes and this->toxic_spikes == other.toxic_spikes and this->stealth_rock == other.stealth_rock and this->me == other.me;
	}
	
	Team (bool isme) :
		active (member),
		counter (0),

		light_screen (0),
		lucky_chant (0),
		mist (0),
		reflect (0),
		safeguard (0),
		tailwind (0),

		wish (0),

		spikes (0),
		toxic_spikes (0),
		stealth_rock (false),
	
		replacement (0),
		me (isme)
		{}
};

std::string search (std::ifstream &file, std::string &output2, const std::string &data);
void loadteam (Team &team, const std::string &name, const Map &map, int detailed [][7]);
void loadpokemon (Team &team, Pokemon &member);
void pokelabteam (Team &team, const std::string &name, const Map &map);
void pokelabpokemon (Team& team, std::ifstream &file, const Map &map);
void poteam (Team &team, const std::string &name);
void popokemon (Team &team, std::ifstream &file, const species pokemon_converter [], const abilities ability_converter [], const items item_converter [], const natures nature_converter [], const moves_list move_converter []);
unsigned poconverter (const std::string &data, const std::string &end, const std::string &line);
void reset_iterators (Team &team);
void reset_iterators_move (Pokemon &member);

}
#endif
