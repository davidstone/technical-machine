// Type function definitions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "team.h"
#include "type.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

bool istype (const Team &team, types type) {
	if ((team.pokemon->type1 == type or team.pokemon->type2 == type) and (type != FLYING or !team.roost))
		return true;
	return false;
}

bool grounded (const Team &team, const Weather &weather) {
	if ((!istype (team, FLYING) and team.pokemon->ability != LEVITATE and team.magnet_rise == 0) or weather.gravity != 0 or team.pokemon->item == IRON_BALL or team.ingrain)
		return true;
	return false;
}

}
