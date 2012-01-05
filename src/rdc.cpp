// Reverse damage calculator interface
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

#include <iostream>
#include <string>
#include <vector>

#include "ability.hpp"
#include "item.hpp"
#include "load_stats.hpp"
#include "move.hpp"
#include "pokemon.hpp"
#include "reversedamage.hpp"
#include "stat.hpp"
#include "status.hpp"
#include "switch.hpp"
#include "team.hpp"
#include "teampredictor.hpp"
#include "type.hpp"
#include "unknown.hpp"
#include "weather.hpp"

// What follows is a temporary file that initializes variables that I don't have functions to initialize yet. It also provides the interface for the reverse damage calculator.

using namespace technicalmachine;

int main () {
	
	int detailed [Species::END] [7] = {{ 0 }};
	std::cerr << "Loading detailed stats.\n";
	detailed_stats (detailed);
	std::cerr << "Loading ai.\n";
	Team ai (true, 6);
	std::cerr << "Loading foe.\n";
	Team foe (false, ai.size);
	std::cerr << "Teams created.\n";

	Pokemon member (INFERNAPE, foe.size);
	foe.pokemon.set.push_back (member);
	foe.pokemon().level = 100;
	foe.pokemon().ability.name = Ability::BLAZE;

	Move move (Move::CLOSE_COMBAT, 3, foe.size);
	foe.pokemon().move.set.insert (foe.pokemon().move.set.begin(), move);
	
	std::cerr << "Loading Pokemon.\n";
	foe.pokemon.set.back().load ();

	Weather weather;
	
	std::cerr << "Switching in the Pokemon.\n";
	ai.replacement = 0;
	switchpokemon (ai, foe, weather);
	foe.replacement = 0;
	switchpokemon (foe, ai, weather);
	calculate_defending_stat (foe, ai, weather);
	calculate_speed (ai, weather);
	
	std::vector<Unknown> hidden;
	unknown (hidden);
	
	unsigned damage = 660;
	std::cerr << "Reverse damage calculation 1\n";
	std::cerr << "hidden.size (): " << hidden.size () << '\n';
	reversedamagecalculator (foe, ai, weather, damage, hidden);
	std::cerr << "Reverse damage calculation 2\n";
	std::cerr << "hidden.size (): " << hidden.size () << '\n';
	damage = 668;
	reversedamagecalculator (foe, ai, weather, damage, hidden);
	std::cerr << "hidden.size (): " << hidden.size () << '\n';
	
	Item::Items thingy = Item::END;
	std::string output = "";
	for (Unknown const & unknown : hidden) {
		if (unknown.hpev == 0 and unknown.speev == 0) {
			if (thingy != unknown.item) {
				output += '\n' + std::to_string (static_cast <int> (unknown.item)) + ": \n";
				thingy = static_cast <Item::Items> (unknown.item);
			}
			output += std::to_string (static_cast <int> (unknown.atkev) * 4);
			if (unknown.nature == static_cast<unsigned char> (Nature::NAUGHTY))
				output += "(+)";
			else if (unknown.nature == static_cast<unsigned char> (Nature::DOCILE))
				output += "(-)";
			output += ", ";
		}
	}
	output += '\n';
	std::cout << output;
	return 0;
}
