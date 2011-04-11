// Miscellaneous functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "simple.h"
#include "ability.h"
#include "item.h"
#include "pokemon.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

bool istype (const pokemon &member, types type) {
	if ((member.type1 == type or member.type2 == type) and (type != FLYING or member.roost == false))
		return true;
	return false;
}

void heal (pokemon &member, int denominator, int numerator) {
	if (0 != member.hp.stat) {
		if ((denominator > 0 and denominator / numerator < member.hp.max) or (denominator < 0 and -denominator / numerator < member.hp.max and MAGIC_GUARD != member.ability))
			member.hp.stat += member.hp.max * numerator / denominator;
		else if (denominator > 0)	// If it would normally heal less than 1, heal 1
			++member.hp.stat;
		else if (MAGIC_GUARD != member.ability)	// If it would normally damage less than 1, damage 1
			--member.hp.stat;
		if (member.hp.stat > member.hp.max)
			member.hp.stat = member.hp.max;
	}
}

void decrement (char &n) {
	if (n > 0)
		--n;
}

bool grounded (const pokemon &member, const Weather &weather) {
	if ((!istype (member, FLYING) 
	and LEVITATE != member.ability 
	and member.magnet_rise == 0) 
	or 0 != weather.gravity 
	or IRON_BALL == member.item 
	or member.ingrain)
		return true;
	return false;
}

void recoil (pokemon &user, int damage, int denominator) {
	if (user.ability != MAGIC_GUARD and user.ability != ROCK_HEAD) {
		if (damage <= denominator)
			--user.hp.stat;
		else
			user.hp.stat -= damage / denominator;
		if (user.hp.stat < 0)
			user.hp.stat = 0;
	}
}

}
