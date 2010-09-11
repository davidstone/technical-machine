// Reverse damage calculator interface
// Copyright 2010 David Stone
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
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "type.h"
#include "weather.h"
#include "stat.cpp"
#include "damage.cpp"
#include "unknown.cpp"
#include "reversedamage.cpp"
#include "team.cpp"
#include "move.cpp"

// What follows is a temporary file that initializes variables that I don't have functions to initialize yet. It also provides the interface for the reverse damage calculator.

int main () {

	teams ai;
	loadteam (ai, "team.sbt");
	teams foe;
	loadteam (foe, "");
	ai.me = true;
	foe.me = false;

	weathers weather;
	weather.trick_room = 0;
	weather.fog = false;
	weather.gravity = 0;
	weather.uproar = 0;
	weather.hail = 0;
	weather.sun = 0;
	weather.sand = 0;
	weather.rain = 0;
	
	pokemon member;
	foe.member.push_back (member);
	foe.active = foe.member.begin();
	foe.active->pokemon = INFERNAPE;
	foe.active->mass = mass [foe.active->pokemon];
	foe.active->level = 100;
	foe.active->hp.base = base_stat [foe.active->pokemon][0];
	foe.active->atk.base = base_stat [foe.active->pokemon][1];
	foe.active->def.base = base_stat [foe.active->pokemon][2];
	foe.active->spa.base = base_stat [foe.active->pokemon][3];
	foe.active->spd.base = base_stat [foe.active->pokemon][4];
	foe.active->spe.base = base_stat [foe.active->pokemon][5];
	hitpoints (*foe.active);
	foe.active->hp.stat = foe.active->hp.max;
	foe.active->ability = BLAZE;
	foe.active->status = NO_STATUS;
	foe.active->gender = MALE;
	foe.active->type1 = get_pokemon_type [foe.active->pokemon][0];
	foe.active->type2 = get_pokemon_type [foe.active->pokemon][1];
	foe.active->def.ev = 0;
	foe.active->spa.ev = 0;
	foe.active->spd.ev = 0;
	foe.active->spe.ev = 0;

	moves move;
	move.name = CLOSE_COMBAT;
	move.type = FIGHTING;
	move.physical = true;
	move.ch = false;
	move.times_used = 0;
	move.pp_max = 8;
	move.pp = move.pp_max;
	move.priority = move_priority [move.name];
	move.basepower = base_power [static_cast <short> (move.name)];
	foe.active->moveset.push_back (move);
	foe.active->move = foe.active->moveset.begin();
	ai.active->move = ai.active->moveset.begin();

	switchpokemon (ai, *ai.active, weather);
	switchpokemon (foe, *foe.active, weather);
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
