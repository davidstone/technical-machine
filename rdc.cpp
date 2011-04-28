// Reverse damage calculator interface
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "ability.h"
#include "item.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "reversedamage.h"
#include "statfunction.h"
#include "status.h"
#include "switch.h"
#include "team.h"
#include "teampredictor.h"
#include "type.h"
#include "unknown.h"
#include "weather.h"

// What follows is a temporary file that initializes variables that I don't have functions to initialize yet. It also provides the interface for the reverse damage calculator.

using namespace technicalmachine;

int main () {
	
	Map map;
	int detailed [END_SPECIES] [7];
	detailed_stats (map, detailed);
	Team ai (true);
	loadteam (ai, "teams/ai.sbt", map, detailed);

	Team foe (false);

	Pokemon member (INFERNAPE);
	foe.active.set.push_back (member);
	foe.active->level = 100;
	foe.active->ability = BLAZE;

	Move move (CLOSE_COMBAT, 3);
	foe.active->move.set.push_back (move);

	loadteam (foe, "", map, detailed);

	Weather weather;
	
	ai.replacement = 0;
	switchpokemon (ai, *foe.active, weather);
	foe.replacement = 0;
	switchpokemon (foe, *ai.active, weather);
	defense (*foe.active, *ai.active, weather);
	speed (ai, weather);
	
	std::vector<Unknown> hidden;
	unknown (hidden);
	
	int damage = 660;
	reversedamagecalculator (foe, ai, weather, damage, hidden);
	damage = 668;
	reversedamagecalculator (foe, ai, weather, damage, hidden);
	
	items thingy = END_ITEM;
	std::string output = "";
	for (std::vector<Unknown>::iterator it = hidden.begin(); it != hidden.end(); ++it) {
		if (it->hpev == 0 and it->speev == 0) {
			if (thingy != it->item) {
				output += '\n' + boost::lexical_cast <std::string> (static_cast <int> (it->item)) + ": \n";
				thingy = static_cast <items> (it->item);
			}
			output += boost::lexical_cast <std::string> (static_cast <int> (it->atkev) * 4);
			if (static_cast<unsigned char> (NAUGHTY) == it->nature)
				output += "(+)";
			else if (static_cast<unsigned char> (MODEST) == it->nature)
				output += "(-)";
			output += ", ";
		}
	}
	output += '\n';
	std::cout << output;
	return 0;
}
