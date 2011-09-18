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
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

void burn (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status == NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::WATER_VEIL and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, FIRE)) {
		target.pokemon->status = BURN;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			burn (target, user, weather);
	}
}

void freeze (Pokemon const & user, Team & target, Weather const & weather) {
	if (target.pokemon->status == NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or target.pokemon->ability.name != Ability::MAGMA_ARMOR) and weather.sun == 0 and !is_type (target, ICE))
		target.pokemon->status = FREEZE;
}

void paralyze (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (target.status == NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or (target.ability.name != Ability::LIMBER and (target.ability.name != Ability::LEAF_GUARD or weather.sun == 0)))) {
		target.status = PARALYSIS;
		if (target.ability.name == Ability::SYNCHRONIZE)
			paralyze (target, user, weather);
	}
}

void sleep (Pokemon const & user, Pokemon & target, Weather const & weather) {
	if (target.status == NO_STATUS and weather.uproar == 0 and (user.ability.name == Ability::MOLD_BREAKER or (target.ability.name != Ability::INSOMNIA and (target.ability.name != Ability::LEAF_GUARD or weather.sun == 0))))
		target.status = SLEEP;
}

void poison_normal (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status == NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::IMMUNITY and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, POISON) and !is_type (target, STEEL)) {
		target.pokemon->status = POISON_NORMAL;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

void poison_toxic (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status == NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::IMMUNITY and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, POISON) and !is_type (target, STEEL)) {
		target.pokemon->status = POISON_TOXIC;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

}
