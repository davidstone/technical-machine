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
#include "pokemon.h"

namespace technicalmachine {

void heal (Pokemon &member, int denominator, int numerator) {
	if (0 != member.hp.stat) {
		if ((denominator > 0 and denominator / numerator < member.hp.max) or (denominator < 0 and -denominator / numerator < member.hp.max and member.ability != MAGIC_GUARD))
			member.hp.stat += member.hp.max * numerator / denominator;
		else if (denominator > 0)	// If it would normally heal less than 1, heal 1
			++member.hp.stat;
		else if (member.ability != MAGIC_GUARD)	// If it would normally damage less than 1, damage 1
			--member.hp.stat;
		if (member.hp.stat > member.hp.max)
			member.hp.stat = member.hp.max;
	}
}

}
