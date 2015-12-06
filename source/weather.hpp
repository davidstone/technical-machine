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

#pragma once

#include "hash.hpp"
#include "operators.hpp"
#include "status.hpp"

#include <bounded_integer/bounded_integer.hpp>

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

	auto trick_room() const -> bool;
	auto fog() const -> bool;
	auto gravity() const -> bool;
	auto uproar() const -> bool;
	auto hail() const -> bool;
	auto sun() const -> bool;
	auto sand() const -> bool;
	auto rain() const -> bool;

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

	constexpr auto hash() const noexcept {
		// There are a lot of illegal values (such as sun having 4 turns left
		// and rain having 3 turns left), and so it should be possible to write
		// a collision-free hash for weather that needs a much smaller range.
		return technicalmachine::hash(
			m_trick_room,
			m_fog,
			m_gravity,
			m_uproar,
			m_hail,
			m_sun,
			m_sand,
			m_rain
		);
	}


	friend auto operator==(Weather lhs, Weather rhs) -> bool;
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

constexpr auto hash(Weather const weather) noexcept {
	return weather.hash();
}

}	// namespace technicalmachine
