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

#include <vector>
#include "pokemon.h"

namespace technicalmachine {

struct teams {
	std::string player;						// The player's name
	std::vector<pokemon> member;			// All Pokemon on the team
	std::vector<pokemon>::iterator active;	// An iterator to the Pokemon that is currently out
	
	pokemon ddfs;		// Information about the attacker at the time of using Doom Desire / Future Sight
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

	bool me;				// Is this my team?
	
	unsigned char replacement;		// If a Pokemon faints, what Pokemon should replace it?
	
	bool operator== (const teams &other) const {
		if (this->member.size() != other.member.size())
			return false;
		for (size_t n = 0; n != this->member.size(); ++n) {
			if (this->member.at (n) != other.member.at (n))
				return false;
		}
		return this->active->name == other.active->name and this->counter == other.counter and this->light_screen == other.light_screen and this->lucky_chant == other.lucky_chant and this->mist == other.mist and this->reflect == other.reflect and this->safeguard == other.safeguard and this->tailwind == other.tailwind and this->wish == other.wish and this->spikes == other.spikes and this->toxic_spikes == other.toxic_spikes and this->stealth_rock == other.stealth_rock and this->me == other.me;
	}
};

std::string search (std::ifstream &file, std::string &output2, const std::string &data);
void loadteam (teams &team, const std::string &name, const Map &map, int detailed [][7]);
void loadpokemon (teams &team, pokemon &member);
void loadmove (moves &move);
void pokelabteam (teams &team, const std::string &name, const Map &map);
void pokelabpokemon (teams& team, std::ifstream &file, const Map &map);
void poteam (teams &team, const std::string &name);
void popokemon (teams &team, std::ifstream &file, const species pokemon_converter [], const abilities ability_converter [], const items item_converter [], const natures nature_converter [], const moves_list move_converter []);
unsigned poconverter (const std::string &data, const std::string end, const std::string &line);

}
#endif
