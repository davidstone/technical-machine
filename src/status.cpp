// Status functions
// Copyright (C) 2012 David Stone
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
#include "type.hpp"
#include "weather.hpp"

namespace technicalmachine {
namespace {

template<Status::Statuses status>
bool status_can_apply (Ability const ability, Pokemon const & target, Weather const & weather) {
	return target.status.is_clear() and
			(ability.ignores_blockers() or !target.ability.blocks_status<status> (weather)) and
			!target.type.blocks_status<status>() and
			!weather.blocks_status<status>();
}

constexpr bool status_is_reflectable (Status::Statuses const status) {
	return status == Status::BURN or status == Status::PARALYSIS or status == Status::POISON or status == Status::POISON_TOXIC;
}

template<Status::Statuses status>
void apply_status (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (status_can_apply<status> (user.ability, target, weather)) {
		target.status.name = status;
		if (status_is_reflectable (status) and target.ability.reflects_status())
			apply_status<status> (target, user, weather);
	}
}

}	// unnamed namespace

Status::Status ():
	name (NO_STATUS) {
}

void Status::clear () {
	name = NO_STATUS;
}

bool Status::is_clear() const {
	return name == NO_STATUS;
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

void Status::burn (Pokemon & user, Pokemon & target, Weather const & weather) {
	apply_status<BURN> (user, target, weather);
}

void Status::freeze (Pokemon const & user, Pokemon & target, Weather const & weather) {
	// apply_status will not modify user because freeze is not reflectable.
	apply_status<FREEZE> (const_cast<Pokemon &> (user), target, weather);
}

void Status::paralyze (Pokemon & user, Pokemon & target, Weather const & weather) {
	apply_status<PARALYSIS> (user, target, weather);
}

void Status::sleep (Pokemon const & user, Pokemon & target, Weather const & weather) {
	constexpr Statuses status = SLEEP;
	if (status_can_apply<status> (user.ability, target, weather))
		target.status.name = status;		// Fix
}

void Status::poison (Pokemon & user, Pokemon & target, Weather const & weather) {
	apply_status<POISON> (user, target, weather);
}

void Status::poison_toxic (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (status_can_apply<POISON> (user.ability, target, weather)) {
		target.status.name = POISON_TOXIC;
		if (target.ability.reflects_status())
			poison (target, user, weather);
	}
}

}	// namespace technicalmachine
