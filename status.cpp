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

#include <string>
#include "status.h"
#include "ability.h"
#include "pokemon.h"
#include "team.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

Status::Status ():
	name (NO_STATUS) {
}

bool Status::is_sleeping () const {
	switch (name) {
		case REST:
		case SLEEP:
			return true;
		default:
			return false;
	}
}

void Status::clear () {
	name = NO_STATUS;
}

void burn (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status.name == Status::NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::WATER_VEIL and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, FIRE)) {
		target.pokemon->status.name = Status::BURN;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			burn (target, user, weather);
	}
}

void freeze (Pokemon const & user, Team & target, Weather const & weather) {
	if (target.pokemon->status.name == Status::NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or target.pokemon->ability.name != Ability::MAGMA_ARMOR) and weather.sun == 0 and !is_type (target, ICE))
		target.pokemon->status.name = Status::FREEZE;
}

void paralyze (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (target.status.name == Status::NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or (target.ability.name != Ability::LIMBER and (target.ability.name != Ability::LEAF_GUARD or weather.sun == 0)))) {
		target.status.name = Status::PARALYSIS;
		if (target.ability.name == Ability::SYNCHRONIZE)
			paralyze (target, user, weather);
	}
}

void sleep (Pokemon const & user, Pokemon & target, Weather const & weather) {
	if (target.status.name == Status::NO_STATUS and weather.uproar == 0 and (user.ability.name == Ability::MOLD_BREAKER or (target.ability.name != Ability::INSOMNIA and (target.ability.name != Ability::LEAF_GUARD or weather.sun == 0))))
		target.status.name = Status::SLEEP;		// Fix
}

void poison_normal (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status.name == Status::NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::IMMUNITY and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, POISON) and !is_type (target, STEEL)) {
		target.pokemon->status.name = Status::POISON_NORMAL;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

void poison_toxic (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon->status.name == Status::NO_STATUS and (user.pokemon->ability.name == Ability::MOLD_BREAKER or (target.pokemon->ability.name != Ability::IMMUNITY and (target.pokemon->ability.name != Ability::LEAF_GUARD or weather.sun == 0))) and !is_type (target, POISON) and !is_type (target, STEEL)) {
		target.pokemon->status.name = Status::POISON_TOXIC;
		if (target.pokemon->ability.name == Ability::SYNCHRONIZE)
			poison_normal (target, user, weather);
	}
}

std::string Status::get_name () const {
	static std::string const status_name [] = { "No status", "Burn", "Freeze", "Paralysis", "Poison", "Toxic", "Rest", "Sleep", "END_STATUS" };
	return status_name [name];
}
}
