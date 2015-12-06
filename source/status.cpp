// Copyright (C) 2015 David Stone
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

template<Statuses status>
auto status_can_apply(Ability const ability, Pokemon const & target, Weather const weather) -> bool {
	return is_clear(get_status(target)) and
		(ability.ignores_blockers() or !get_ability(target).blocks_status<status>(weather)) and
		!blocks_status<status>(get_type(target)) and
		!weather.blocks_status(status);
}

constexpr auto status_is_reflectable (Statuses const status) -> bool {
	return status == Statuses::burn or status == Statuses::paralysis or status == Statuses::poison or status == Statuses::poison_toxic;
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
		default:
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

template<Statuses base_status, Statuses real_status>
auto Status::apply(Pokemon & user, Pokemon & target, Weather const weather) -> void {
	if (status_can_apply<real_status>(get_ability(user), target, weather)) {
		get_status(target).m_status = real_status;
		if (status_is_reflectable(base_status) and get_ability(target).reflects_status()) {
			apply<base_status>(target, user, weather);
		}
	}
}

template<Statuses base_status>
auto Status::apply(Pokemon & target, Weather const weather) -> void {
	apply<base_status>(target, target, weather);
}

template auto Status::apply<Statuses::burn>(Pokemon & user, Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::freeze>(Pokemon & user, Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::paralysis>(Pokemon & user, Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::poison>(Pokemon & user, Pokemon & target, Weather const weather) -> void;

template<>
auto Status::apply<Statuses::poison_toxic>(Pokemon & user, Pokemon & target, Weather const weather) -> void {
	apply<Statuses::poison, Statuses::poison_toxic>(user, target, weather);
}

template<>
auto Status::apply<Statuses::sleep>(Pokemon & user, Pokemon & target, Weather const weather) -> void {
	constexpr auto status_to_apply = Statuses::sleep;
	if (status_can_apply<status_to_apply>(get_ability(user), target, weather)) {
		auto & status = get_status(target);
		status.m_status = status_to_apply;
		status.m_turns_already_slept = 0_bi;
	}
}

template auto Status::apply<Statuses::burn>(Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::freeze>(Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::paralysis>(Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::poison>(Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::poison_toxic>(Pokemon & target, Weather const weather) -> void;
template auto Status::apply<Statuses::sleep>(Pokemon & target, Weather const weather) -> void;


auto Status::shift (Pokemon & user, Pokemon & target, Weather const weather) -> void {
	switch (get_status(user).name()) {
		case Statuses::burn:
			apply<Statuses::burn>(user, target, weather);
			break;
		case Statuses::paralysis:
			apply<Statuses::paralysis>(user, target, weather);
			break;
		case Statuses::poison:
			apply<Statuses::poison>(user, target, weather);
			break;
		case Statuses::poison_toxic:
			apply<Statuses::poison_toxic>(user, target, weather);
			break;
		case Statuses::sleep_rest:		// Fix
		case Statuses::sleep:
			apply<Statuses::sleep>(user, target, weather);
			break;
		default:
			break;
	}
	using std::swap;
	swap(get_status(user).m_turns_already_slept, get_status(user).m_turns_already_slept);
	get_status(user) = Status{};
}

auto Status::increase_sleep_counter(Ability const & ability, bool awaken) -> void {
	if (awaken) {
		m_turns_already_slept = bounded::none;
		m_status = Statuses::clear;
	}
	else {
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
