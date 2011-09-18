// Function to heal Pokemon
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "heal.h"
#include "ability.h"
#include "damage.h"
#include "pokemon.h"

namespace technicalmachine {

void heal (Pokemon &member, int denominator, unsigned numerator) {
	if (member.hp.stat != 0) {
		if (denominator > 0) {
			if (member.hp.max * numerator <= static_cast <unsigned> (2 * denominator - 1))
				++member.hp.stat;
			else
				member.hp.stat += member.hp.max * numerator / denominator;
			if (member.hp.stat > member.hp.max)
				member.hp.stat = member.hp.max;
		}
		else {
			if (member.ability.name != Ability::MAGIC_GUARD) {
				if (member.hp.max * numerator <= static_cast <unsigned> (-2 * denominator - 1))
					-- member.hp.stat;
				else
					damage_side_effect (member, member.hp.max * numerator / (-denominator));
			}
		}
	}
}

}
