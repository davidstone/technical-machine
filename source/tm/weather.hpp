// Copyright (C) 2018 David Stone
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

#pragma once

#include "operators.hpp"
#include "status.hpp"

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

// Weather is the set of things that are not specific to either team.

// Due to the "acid weather" effect, multiple weathers can be in effect at the
// same time. The order of the weathers is their order when all effects are
// active.
//
// Uproar is considered Weather because it can be activated without being
// associated with any particular Pokemon / Team.

struct Weather {
	enum class Duration : int8_t { standard = 5, extended = 8, permanent = -1 };
	
	constexpr Weather() {}

	constexpr auto trick_room() const {
		return m_trick_room != 0_bi;
	}
	constexpr auto fog() const {
		return m_fog;
	}
	constexpr auto gravity() const {
		return m_gravity != 0_bi;
	}
	constexpr auto uproar() const {
		return m_uproar != 0_bi;
	}
	constexpr auto hail() const {
		return m_hail != 0_bi;
	}
	constexpr auto sun() const {
		return m_sun != 0_bi;
	}
	constexpr auto sand() const {
		return m_sand != 0_bi;
	}
	constexpr auto rain() const {
		return m_rain != 0_bi;
	}


	auto advance_one_turn() -> void;

	auto activate_trick_room() -> void;
	auto activate_fog() -> void;
	auto deactivate_fog() -> void;
	auto activate_gravity() -> void;
	template<typename T>
	auto activate_uproar(T const duration) {
		if (m_uproar < duration) {
			m_uproar = duration;
		}
	}
	auto activate_hail(Duration duration) -> void;
	auto activate_sun(Duration duration) -> void;
	auto activate_sand(Duration duration) -> void;
	auto activate_rain(Duration duration) -> void;
	auto activate_hail(bool is_extended) -> void;
	auto activate_sun(bool is_extended) -> void;
	auto activate_sand(bool is_extended) -> void;
	auto activate_rain(bool is_extended) -> void;

	auto blocks_status(Statuses status) const -> bool;

	friend constexpr auto operator==(Weather const lhs, Weather const rhs) {
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

private:
	template<typename T>
	auto activate_weather(T & primary, bool const is_extended) {
		activate_weather(primary, is_extended ? Duration::extended : Duration::standard);
	}
	template<typename T>
	auto activate_weather(T & primary, Duration const duration) {
		if (primary == 0_bi or duration == Duration::permanent) {
			m_hail = 0_bi;
			m_sand = 0_bi;
			m_rain = 0_bi;
			m_sun = 0_bi;
			primary = static_cast<T>(duration);
		}
	}

	// The number of turns remaining on that weather. A value of -1 indicates
	// permanent weather. Fog is a bool because it only exists as a permanent
	// weather condition.

	bounded::integer<0, 5> m_trick_room = 0_bi;
	bool m_fog = false;
	bounded::integer<-1, 5> m_gravity = 0_bi;
	bounded::integer<-1, 1> m_uproar = 0_bi;
	using Standard = bounded::integer<-1, static_cast<intmax_t>(Duration::extended)>;
	Standard m_hail = 0_bi;
	Standard m_sun = 0_bi;
	Standard m_sand = 0_bi;
	Standard m_rain = 0_bi;
};

}	// namespace technicalmachine
