// Copyright (C) 2016 David Stone
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
#include "hash.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

#include "type/type.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto status_can_apply(Statuses const status, Ability const ability, Pokemon const & target, Weather const weather) {
	return
		is_clear(get_status(target)) and
		(ability.ignores_blockers() or !get_ability(target).blocks_status(status, weather)) and
		!blocks_status(get_type(target), status) and
		!weather.blocks_status(status);
}

constexpr auto reflected_status(Statuses const status) -> bounded::optional<Statuses> {
	switch (status) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
		return status;
	case Statuses::poison_toxic:
		return Statuses::poison;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::sleep_rest:
	case Statuses::END:
		return bounded::none;
	}
}

}	// namespace

auto is_clear(Status const status) -> bool {
	return status.name() == Statuses::clear;
}
auto is_frozen(Status const status) -> bool {
	return status.name() == Statuses::freeze;
}
auto is_sleeping(Status const status) -> bool {
	switch (status.name()) {
		case Statuses::sleep_rest:
		case Statuses::sleep:
			return true;
		default:
			return false;
	}
}
auto is_sleeping_due_to_other(Status const status) -> bool {
	return status.name() == Statuses::sleep;
}
auto lowers_speed(Status const status, Ability const & ability) -> bool {
	return status.name() == Statuses::paralysis and !ability.blocks_paralysis_speed_penalty();
}
auto weakens_physical_attacks(Status const status) -> bool {
	return status.name() == Statuses::burn;
}
auto boosts_facade(Status const status) -> bool {
	switch (status.name()) {
	case Statuses::burn:
	case Statuses::paralysis:
	case Statuses::poison:
	case Statuses::poison_toxic:
		return true;
	case Statuses::clear:
	case Statuses::freeze:
	case Statuses::sleep:
	case Statuses::sleep_rest:
	case Statuses::END:
		return false;
	}
}
auto boosts_smellingsalt(Status const status) -> bool {
	return status.name() == Statuses::paralysis;
}

auto Status::rest() -> void {
	m_status = Statuses::sleep_rest;
	m_turns_already_slept = 0_bi;
}

auto apply(Statuses const status, Pokemon & user, Pokemon & target, Weather const weather) -> void {
	assert(status != Statuses::clear);
	assert(status != Statuses::sleep_rest);
	if (status_can_apply(status, get_ability(user), target, weather)) {
		get_status(target).m_status = status;
		if (status == Statuses::sleep) {
			get_status(target).m_turns_already_slept = 0_bi;
		}
		auto const reflected = reflected_status(status);
		if (reflected and get_ability(target).reflects_status()) {
			apply(*reflected, target, user, weather);
		}
	}
}

auto apply(Statuses const status, Pokemon & target, Weather const weather) -> void {
	apply(status, target, target, weather);
}

auto shift_status(Pokemon & user, Pokemon & target, Weather const weather) -> void {
	auto & status = get_status(user);
	switch (status.name()) {
		case Statuses::burn:
		case Statuses::paralysis:
		case Statuses::poison:
		case Statuses::poison_toxic:
			apply(status.name(), user, target, weather);
			break;
		case Statuses::sleep:
		case Statuses::sleep_rest:		// Fix
			apply(Statuses::sleep, user, target, weather);
			get_status(target).m_turns_already_slept = status.m_turns_already_slept;
			break;
		default:
			break;
	}
	status = Status{};
}

auto Status::increase_sleep_counter(Ability const & ability, bool awaken) -> void {
	if (awaken) {
		m_turns_already_slept = bounded::none;
		m_status = Statuses::clear;
	} else {
		*m_turns_already_slept += BOUNDED_CONDITIONAL(ability.wakes_up_early(), 2_bi, 1_bi);
	}
}

auto operator== (Status const lhs, Status const rhs) -> bool {
	return
		lhs.name() == rhs.name() and
		lhs.m_turns_already_slept == rhs.m_turns_already_slept;
}

namespace {

constexpr auto max_sleep_turns = 4_bi;
using DefiniteSleepCounter = bounded::integer<0, static_cast<intmax_t>(max_sleep_turns)>;

// It's possible to acquire Early Bird in the middle of a sleep
auto early_bird_probability(DefiniteSleepCounter const turns_slept) {
	switch (turns_slept.value()) {
	case 0:
		return 1.0 / 4.0;
	case 1:
		return 1.0 / 2.0;
	case 2:
		return 2.0 / 3.0;
	default:	// case 3, 4
		return 1.0;
	}
}

auto non_early_bird_probability(DefiniteSleepCounter const turns_slept) {
	return (turns_slept == 0_bi) ? 0.0 : (1.0 / static_cast<double>(max_sleep_turns + 1_bi - turns_slept));
}

}	// namespace

auto Status::awaken_probability(Ability const & ability) const -> AwakenProbability {
	static_assert(std::is_same<DefiniteSleepCounter, SleepCounter::value_type>::value, "Incorrect sleep counter type.");
	if (!m_turns_already_slept) {
		return 0.0;
	}
	return ability.wakes_up_early() ?
		early_bird_probability(*m_turns_already_slept) :
		non_early_bird_probability(*m_turns_already_slept)
	;
}

}	// namespace technicalmachine
