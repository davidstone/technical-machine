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
#include "simple.h"
#include "status.h"
#include "statusfunction.h"
#include "weather.h"

namespace technicalmachine {

void burn (pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or (target.ability != WATER_VEIL and (target.ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, FIRE)) {
		target.status = BURN;
		if (target.ability == SYNCHRONIZE)
			burn (target, user, weather);
	}
}

void freeze (const pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or target.ability != MAGMA_ARMOR) and weather.sun == 0 and !istype (target, ICE))
		target.status = FREEZE;
}

void paralyze (pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or (target.ability != LIMBER and (target.ability != LEAF_GUARD or weather.sun == 0)))) {
		target.status = PARALYSIS;
		if (target.ability == SYNCHRONIZE)
			paralyze (target, user, weather);
	}
}

void sleep (const pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and weather.uproar == 0 and (user.ability == MOLD_BREAKER or (target.ability != INSOMNIA and (target.ability != LEAF_GUARD or weather.sun == 0))))
		target.status = SLEEP;
}

void poison_normal (pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or (target.ability != IMMUNITY and (target.ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, POISON) and !istype (target, STEEL)) {
		target.status = POISON_NORMAL;
		if (target.ability == SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

void poison_toxic (pokemon &user, pokemon &target, const Weather &weather) {
	if (target.status == NO_STATUS and (user.ability == MOLD_BREAKER or (target.ability != IMMUNITY and (target.ability != LEAF_GUARD or weather.sun == 0))) and !istype (target, POISON) and !istype (target, STEEL)) {
		target.status = POISON_TOXIC;
		if (target.ability == SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

}
