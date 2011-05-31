// Pokemon functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "active.h"
#include "movefunction.h"
#include "pokemon.h"
#include "species.h"
#include "stat.h"
#include "statfunction.h"

namespace technicalmachine {

Pokemon::Pokemon (species const &member, unsigned size) : 
	name (member),
	ability (END_ABILITY),
	gender (MALE),		// No sexism here!
	item (END_ITEM),
	status (NO_STATUS),
	type1 (get_pokemon_type [name][0]),
	type2 (get_pokemon_type [name][1]),
	hp (name, level, HP),
	atk (name, level, ATK),
	def (name, level, DEF),
	spa (name, level, SPA),
	spd (name, level, SPD),
	spe (name, level, SPE),

	fainted (false),
	level (100),
	mass (get_mass [name]),
	sleep (0),

	happiness (255) {
		Move struggle (STRUGGLE, 0);
		move.set.push_back (struggle);
		// A Pokemon has a new "Switch" move for each Pokemon in the party.
		if (size > 1) {
			for (size_t index = SWITCH0; index - SWITCH0 < size; ++index) {
				Move switchn (static_cast<moves_list> (index), 0);
				move.set.push_back (switchn);
			}
		}
}

void Pokemon::load () {
	hp.max = hitpoints (*this);
	hp.stat = hp.max;
}

}
