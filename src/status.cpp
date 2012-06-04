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

#include <algorithm>

#include "ability.hpp"
#include "type.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

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

}	// unnamed namespace

Status::Status ():
	status (NO_STATUS),
	turns_already_slept (0) {
}

Status::Statuses Status::name() const {
	return status;
}

void Status::clear () {
	status = NO_STATUS;
	turns_already_slept = 0;
}

bool Status::is_clear() const {
	return name() == NO_STATUS;
}

bool Status::is_frozen() const {
	return name() == FREEZE;
}

bool Status::is_sleeping () const {
	switch (name()) {
		case REST:
		case SLEEP:
			return true;
		default:
			return false;
	}
}

bool Status::is_sleeping_due_to_other() const {
	return name() == SLEEP;
}

bool Status::lowers_speed(Ability const & ability) const {
	return name() == PARALYSIS and !ability.blocks_paralysis_speed_penalty();
}

bool Status::weakens_physical_attacks() const {
	return name() == BURN;
}

bool Status::boosts_facade() const {
	switch (name()) {
		case BURN:
		case PARALYSIS:
		case POISON:
		case POISON_TOXIC:
			return true;
		default:
			return false;
	}
}

bool Status::boosts_smellingsalt() const {
	return name() == PARALYSIS;
}

void Status::rest() {
	status = REST;
	turns_already_slept = 3;	// TODO: count up instead of down
}

template<Status::Statuses base_status, Status::Statuses real_status>
void Status::apply (Pokemon & user, Pokemon & target, Weather const & weather) {
	if (status_can_apply<base_status>(user.ability, target, weather)) {
		target.status.status = real_status;
		if (status_is_reflectable(base_status) and target.ability.reflects_status())
			apply<base_status>(target, user, weather);
	}
}

template<Status::Statuses base_status>
void Status::apply (Pokemon & target, Weather const & weather) {
	apply<base_status>(target, target, weather);
}

template void Status::apply<Status::BURN>(Pokemon & user, Pokemon & target, Weather const & weather);
template void Status::apply<Status::FREEZE>(Pokemon & user, Pokemon & target, Weather const & weather);
template void Status::apply<Status::PARALYSIS>(Pokemon & user, Pokemon & target, Weather const & weather);
template void Status::apply<Status::POISON>(Pokemon & user, Pokemon & target, Weather const & weather);

template<>
void Status::apply<Status::POISON_TOXIC>(Pokemon & user, Pokemon & target, Weather const & weather) {
	apply<Status::POISON_TOXIC, Status::POISON>(user, target, weather);
}

template<>
void Status::apply<Status::SLEEP>(Pokemon & user, Pokemon & target, Weather const & weather) {
	constexpr Statuses status = SLEEP;
	if (status_can_apply<status> (user.ability, target, weather))
		target.status.status = status;		// Fix
}

template void Status::apply<Status::BURN>(Pokemon & target, Weather const & weather);
template void Status::apply<Status::FREEZE>(Pokemon & target, Weather const & weather);
template void Status::apply<Status::PARALYSIS>(Pokemon & target, Weather const & weather);
template void Status::apply<Status::POISON>(Pokemon & target, Weather const & weather);
template void Status::apply<Status::POISON_TOXIC>(Pokemon & target, Weather const & weather);
template void Status::apply<Status::SLEEP>(Pokemon & target, Weather const & weather);


void Status::shift (Pokemon & user, Pokemon & target, Weather const & weather) {
	switch (user.status.name()) {
		case Status::BURN:
			apply<BURN>(user, target, weather);
			break;
		case Status::PARALYSIS:
			apply<PARALYSIS>(user, target, weather);
			break;
		case Status::POISON:
			apply<POISON>(user, target, weather);
			break;
		case Status::POISON_TOXIC:
			apply<POISON_TOXIC>(user, target, weather);
			break;
		case Status::REST:		// Fix
		case Status::SLEEP:
			apply<SLEEP>(user, target, weather);
			break;
		default:
			break;
	}
	using std::swap;
	swap(user.status.turns_already_slept, user.status.turns_already_slept);
	user.status.clear();
}

void Status::increase_sleep_counter (Ability const & ability, bool awaken) {
	if (awaken) {
		turns_already_slept = 0;
		clear();
	}
	else {
		turns_already_slept += ability.wakes_up_early() ? 2 : 1;
	}
}

bool operator== (Status const lhs, Status const rhs) {
	return lhs.name() == rhs.name() and
			lhs.turns_already_slept == rhs.turns_already_slept;
}

bool operator!= (Status const lhs, Status const rhs) {
	return !(lhs == rhs);
}

bool Status::can_awaken(Ability const & ability) const {
	return awaken_numerator(ability) > min_sleep_turns();
}

unsigned Status::awaken_numerator (Ability const & ability) const {
	return static_cast<unsigned>(turns_already_slept) + ability.wakes_up_early();
}

unsigned Status::min_sleep_turns () {
	return 1;
}

unsigned Status::max_sleep_turns () {
	return 4;
}

uint64_t Status::hash() const {
	return name() + END *
			static_cast<uint64_t>(turns_already_slept);
}

uint64_t Status::max_hash() {
	return END + (max_sleep_turns() + 1);
}

}	// namespace technicalmachine
