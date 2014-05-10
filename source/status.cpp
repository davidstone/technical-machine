// Status functions
// Copyright (C) 2014 David Stone
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
#include "rational.hpp"
#include "weather.hpp"

#include "pokemon/pokemon.hpp"

#include "type/type.hpp"

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<Status::Statuses status>
auto status_can_apply(Ability const ability, Pokemon const & target, Weather const & weather) -> bool {
	return is_clear(get_status(target)) and
		(ability.ignores_blockers() or !get_ability(target).blocks_status<status>(weather)) and
		!get_type(target).blocks_status<status>() and
		!weather.blocks_status<status>();
}

constexpr auto status_is_reflectable (Status::Statuses const status) -> bool {
	return status == Status::burn or status == Status::paralysis or status == Status::poison or status == Status::poison_toxic;
}

}	// namespace

auto Status::name() const -> Statuses {
	return m_status;
}

auto is_clear(Status const status) -> bool {
	return status.name() == Status::clear;
}
auto is_frozen(Status const status) -> bool {
	return status.name() == Status::freeze;
}
auto is_sleeping(Status const status) -> bool {
	switch (status.name()) {
		case Status::sleep_rest:
		case Status::sleep:
			return true;
		default:
			return false;
	}
}
auto is_sleeping_due_to_other(Status const status) -> bool {
	return status.name() == Status::sleep;
}
auto lowers_speed(Status const status, Ability const & ability) -> bool {
	return status.name() == Status::paralysis and !ability.blocks_paralysis_speed_penalty();
}
auto weakens_physical_attacks(Status const status) -> bool {
	return status.name() == Status::burn;
}
auto boosts_facade(Status const status) -> bool {
	switch (status.name()) {
		case Status::burn:
		case Status::paralysis:
		case Status::poison:
		case Status::poison_toxic:
			return true;
		default:
			return false;
	}
}
auto boosts_smellingsalt(Status const status) -> bool {
	return status.name() == Status::paralysis;
}

auto Status::rest() -> void {
	m_status = Status::sleep_rest;
	m_turns_already_slept = 0_bi;
}

template<Status::Statuses base_status, Status::Statuses real_status>
auto Status::apply(Pokemon & user, Pokemon & target, Weather const & weather) -> void {
	if (status_can_apply<real_status>(get_ability(user), target, weather)) {
		get_status(target).m_status = real_status;
		if (status_is_reflectable(base_status) and get_ability(target).reflects_status()) {
			apply<base_status>(target, user, weather);
		}
	}
}

template<Status::Statuses base_status>
auto Status::apply(Pokemon & target, Weather const & weather) -> void {
	apply<base_status>(target, target, weather);
}

template auto Status::apply<Status::burn>(Pokemon & user, Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::freeze>(Pokemon & user, Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::paralysis>(Pokemon & user, Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::poison>(Pokemon & user, Pokemon & target, Weather const & weather) -> void;

template<>
auto Status::apply<Status::poison_toxic>(Pokemon & user, Pokemon & target, Weather const & weather) -> void {
	apply<Status::poison, Status::poison_toxic>(user, target, weather);
}

template<>
auto Status::apply<Status::sleep>(Pokemon & user, Pokemon & target, Weather const & weather) -> void {
	constexpr auto status_to_apply = sleep;
	if (status_can_apply<status_to_apply>(get_ability(user), target, weather)) {
		auto & status = get_status(target);
		status.m_status = status_to_apply;
		status.m_turns_already_slept = 0_bi;
	}
}

template auto Status::apply<Status::burn>(Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::freeze>(Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::paralysis>(Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::poison>(Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::poison_toxic>(Pokemon & target, Weather const & weather) -> void;
template auto Status::apply<Status::sleep>(Pokemon & target, Weather const & weather) -> void;


auto Status::shift (Pokemon & user, Pokemon & target, Weather const & weather) -> void {
	switch (get_status(user).name()) {
		case Status::burn:
			apply<burn>(user, target, weather);
			break;
		case Status::paralysis:
			apply<paralysis>(user, target, weather);
			break;
		case Status::poison:
			apply<poison>(user, target, weather);
			break;
		case Status::poison_toxic:
			apply<poison_toxic>(user, target, weather);
			break;
		case Status::sleep_rest:		// Fix
		case Status::sleep:
			apply<sleep>(user, target, weather);
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
		m_status = clear;
	}
	else {
		*m_turns_already_slept += BOUNDED_INTEGER_CONDITIONAL(ability.wakes_up_early(), 2_bi, 1_bi);
	}
}

auto operator== (Status const lhs, Status const rhs) -> bool {
	return
		lhs.name() == rhs.name() and
		lhs.m_turns_already_slept == rhs.m_turns_already_slept;
}

auto operator!= (Status const lhs, Status const rhs) -> bool {
	return !(lhs == rhs);
}

namespace {

using DefiniteSleepCounter = bounded::integer<0, 4>;

auto awaken_numerator(DefiniteSleepCounter const turns_slept, Ability const & ability) {
	return turns_slept + BOUNDED_INTEGER_CONDITIONAL(ability.wakes_up_early(), 1_bi, 0_bi);
}

auto can_awaken(DefiniteSleepCounter const turns_slept, Ability const & ability) {
	constexpr auto min_sleep_turns = 1_bi;
	return awaken_numerator(turns_slept, ability) >= min_sleep_turns;
}

}	// namespace

auto Status::awaken_probability(Ability const & ability, bool const awaken) const -> Rational {
	if (!m_turns_already_slept) {
		return Rational(0, 1);
	}
	static constexpr auto max_sleep_turns = std::numeric_limits<SleepCounter::value_type>::max();
	Rational const result(
		BOUNDED_INTEGER_CONDITIONAL(can_awaken(*m_turns_already_slept, ability), 1_bi, 0_bi),
		max_sleep_turns + 1_bi - awaken_numerator(*m_turns_already_slept, ability)
	);
	return awaken ? result : complement(result);
}

auto Status::hash() const -> uint64_t {
	return
		name() + END *
		static_cast<uint64_t>(::technicalmachine::hash(m_turns_already_slept));
}

auto Status::max_hash() const -> uint64_t {
	return END + static_cast<uint64_t>(::technicalmachine::max_hash(m_turns_already_slept));
}

}	// namespace technicalmachine
