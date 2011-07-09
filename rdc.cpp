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
#include "load_stats.h"
#include "move.h"
#include "movefunction.h"
#include "pokemon.h"
#include "reversedamage.h"
#include "stat.h"
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
	Team ai (true, map);
	Team foe (false, map);

	Pokemon member (INFERNAPE, foe.size);
	foe.pokemon.set.push_back (member);
	foe.pokemon->level = 100;
	foe.pokemon->ability = BLAZE;

	Move move (CLOSE_COMBAT, 3);
	foe.pokemon->move.set.insert (foe.pokemon->move.set.begin(), move);

	foe.pokemon.set.back().load ();

	Weather weather;
	
	ai.replacement = 0;
	switchpokemon (ai, foe, weather);
	foe.replacement = 0;
	switchpokemon (foe, ai, weather);
	defense (foe, ai, weather);
	speed (ai, weather);
	
	std::cout << "Creating vector\n";
	std::vector<Unknown> hidden;
	unknown (hidden);
	
	int damage = 660;
	std::cout << "Reverse damage calculation 1\n";
	reversedamagecalculator (foe, ai, weather, damage, hidden);
	std::cout << "Reverse damage calculation 2\n";
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
