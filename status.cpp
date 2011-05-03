// Status functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "pokemon.h"
#include "status.h"
#include "statusfunction.h"
#include "team.h"
#include "typefunction.h"
#include "weather.h"

namespace technicalmachine {

void burn (Team &user, Team &target, const Weather &weather) {
	if (target.active->status == NO_STATUS and (user.active->ability == MOLD_BREAKER or (target.active->ability != WATER_VEIL and (target.active->ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, FIRE)) {
		target.active->status = BURN;
		if (target.active->ability == SYNCHRONIZE)
			burn (target, user, weather);
	}
}

void freeze (const Pokemon &user, Team &target, const Weather &weather) {
	if (target.active->status == NO_STATUS and (user.ability == MOLD_BREAKER or target.active->ability != MAGMA_ARMOR) and weather.sun == 0 and !istype (target, ICE))
		target.active->status = FREEZE;
}

void paralyze (Pokemon &user, Pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or (target.ability != LIMBER and (target.ability != LEAF_GUARD or weather.sun == 0)))) {
		target.status = PARALYSIS;
		if (target.ability == SYNCHRONIZE)
			paralyze (target, user, weather);
	}
}

void sleep (const Pokemon &user, Pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and weather.uproar == 0 and (user.ability == MOLD_BREAKER or (target.ability != INSOMNIA and (target.ability != LEAF_GUARD or weather.sun == 0))))
		target.status = SLEEP;
}

void poison_normal (Team &user, Team &target, const Weather &weather) {
	if (target.active->status == NO_STATUS and (user.active->ability == MOLD_BREAKER or (target.active->ability != IMMUNITY and (target.active->ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, POISON) and !istype (target, STEEL)) {
		target.active->status = POISON_NORMAL;
		if (target.active->ability == SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

void poison_toxic (Team &user, Team &target, const Weather &weather) {
	if (target.active->status == NO_STATUS and (user.active->ability == MOLD_BREAKER or (target.active->ability != IMMUNITY and (target.active->ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, POISON) and !istype (target, STEEL)) {
		target.active->status = POISON_TOXIC;
		if (target.active->ability == SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

}
