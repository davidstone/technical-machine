// Status functions
// Copyright (C) 2011 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include "status.hpp"

#include "ability.hpp"
#include "pokemon.hpp"
#include "team.hpp"
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

void apply_poison_status (Team & user, Team & target, Weather const & weather, Status::Statuses status);

}	// unnamed namespace

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

void Status::burn (Team & user, Team & target, Weather const & weather) {
	if (target.pokemon().status.name == NO_STATUS and (user.pokemon().ability.name == Ability::MOLD_BREAKER or !target.pokemon().ability.blocks_burn (weather)) and !target.pokemon().type.blocks_burn ()) {
		target.pokemon().status.name = BURN;
		if (target.pokemon().ability.name == Ability::SYNCHRONIZE)
			burn (target, user, weather);
	}
}

void Status::freeze (Pokemon const & user, Team & target, Weather const & weather) {
	if (target.pokemon().status.name == NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or !target.pokemon().ability.blocks_freeze ()) and !weather.sun and !target.pokemon().type.blocks_freeze ())
		target.pokemon().status.name = FREEZE;
}

void Status::paralyze (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (target.status.name == NO_STATUS and (user.ability.name == Ability::MOLD_BREAKER or !target.ability.blocks_paralysis (weather))) {
		target.status.name = PARALYSIS;
		if (target.ability.name == Ability::SYNCHRONIZE)
			paralyze (target, user, weather);
	}
}

void Status::sleep (Pokemon const & user, Pokemon & target, Weather const & weather) {
	if (target.status.name == NO_STATUS and !weather.uproar and (user.ability.name == Ability::MOLD_BREAKER or !target.ability.blocks_sleep (weather)))
		target.status.name = SLEEP;		// Fix
}

void Status::poison (Team & user, Team & target, Weather const & weather) {
	apply_poison_status (user, target, weather, POISON);
}

void Status::poison_toxic (Team & user, Team & target, Weather const & weather) {
	apply_poison_status (user, target, weather, POISON_TOXIC);
}

namespace {

void apply_poison_status (Team & user, Team & target, Weather const & weather, Status::Statuses const status) {
	if (target.pokemon().status.name == Status::NO_STATUS and (user.pokemon().ability.name == Ability::MOLD_BREAKER or !target.pokemon().ability.blocks_poison (weather)) and !target.pokemon().type.blocks_poison ()) {
		target.pokemon().status.name = status;
		if (target.pokemon().ability.name == Ability::SYNCHRONIZE)
			Status::poison (target, user, weather);
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
