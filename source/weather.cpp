// Weather functions
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

#include "weather.hpp"
#include "status.hpp"

namespace technicalmachine {
namespace {
template<typename T>
auto conditional_decrement(T & n) -> void {
	if (n > 0_bi) {
		--n;
	}
}
}	// namespace

auto Weather::trick_room() const -> bool {
	return m_trick_room != 0_bi;
}
auto Weather::gravity() const -> bool {
	return m_gravity != 0_bi;
}
auto Weather::uproar() const -> bool {
	return m_uproar != 0_bi;
}
auto Weather::hail() const -> bool {
	return m_hail != 0_bi;
}
auto Weather::sun() const -> bool {
	return m_sun != 0_bi;
}
auto Weather::sand() const -> bool {
	return m_sand != 0_bi;
}
auto Weather::rain() const -> bool {
	return m_rain != 0_bi;
}

auto Weather::advance_one_turn() -> void {
	conditional_decrement(m_trick_room);
	conditional_decrement(m_gravity);
	conditional_decrement(m_uproar);
	conditional_decrement(m_hail);
	conditional_decrement(m_sun);
	conditional_decrement(m_sand);
	conditional_decrement(m_rain);
}

auto Weather::activate_trick_room() -> void {
	m_trick_room = BOUNDED_CONDITIONAL(trick_room(), 0_bi, 5_bi);
}

auto Weather::activate_fog() -> void {
	m_fog = true;
}
auto Weather::deactivate_fog() -> void {
	m_fog = false;
}

auto Weather::activate_gravity() -> void {
	if (!gravity()) {
		m_gravity = 5_bi;
	}
}

auto Weather::activate_hail(Duration const duration) -> void {
	activate_weather(m_hail, duration);
}
auto Weather::activate_sun(Duration const duration) -> void {
	activate_weather(m_sun, duration);
}
auto Weather::activate_sand(Duration const duration) -> void {
	activate_weather(m_sand, duration);
}
auto Weather::activate_rain(Duration const duration) -> void {
	activate_weather(m_rain, duration);
}
auto Weather::activate_hail(bool const is_extended) -> void {
	activate_weather(m_hail, is_extended);
}
auto Weather::activate_sun(bool const is_extended) -> void {
	activate_weather(m_sun, is_extended);
}
auto Weather::activate_sand(bool const is_extended) -> void {
	activate_weather(m_sand, is_extended);
}
auto Weather::activate_rain(bool const is_extended) -> void {
	activate_weather(m_rain, is_extended);
}

auto Weather::blocks_status(Statuses const status) const -> bool {
	switch (status) {
	case Statuses::freeze:
		return sun();
	case Statuses::sleep:
	case Statuses::sleep_rest:
		return uproar();
	default:
		return false;
	}
}

auto operator==(Weather const lhs, Weather const rhs) -> bool {
	return
		lhs.m_trick_room == rhs.m_trick_room and
		lhs.m_fog == rhs.m_fog and
		lhs.m_gravity == rhs.m_gravity and
		lhs.m_uproar == rhs.m_uproar and
		lhs.m_hail == rhs.m_hail and
		lhs.m_sun == rhs.m_sun and
		lhs.m_sand == rhs.m_sand and
		lhs.m_rain == rhs.m_rain;
}

auto operator!=(Weather const lhs, Weather const rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
