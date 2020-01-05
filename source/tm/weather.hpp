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

#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/status.hpp>

#include <bounded/integer.hpp>
#include <bounded/detail/variant/variant.hpp>

#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto weather_is_blocked_by_ability(Ability const ability) {
	switch (ability) {
		case Ability::Air_Lock:
		case Ability::Cloud_Nine:
			return true;
		default:
			return false;
	}
}
constexpr auto weather_is_blocked_by_ability(Ability const ability1, Ability const ability2) {
	return weather_is_blocked_by_ability(ability1) or weather_is_blocked_by_ability(ability2);
}

struct Weather {
private:
	enum class NormalWeather : std::uint8_t { clear, hail, sand, sun, rain };
	static constexpr auto standard = 5_bi;
	static constexpr auto extended = 8_bi;
	static constexpr auto permanent = -1_bi;
	using Short = bounded::integer<0, static_cast<int>(standard)>;
	using Long = bounded::integer<-1, static_cast<int>(extended)>;

	constexpr auto activate_weather(NormalWeather const type, Long const duration) {
		if (type == m_active and duration != permanent) {
			return;
		}
		m_active = type;
		m_turns_remaining = static_cast<Long>(duration);
	}
	constexpr auto activate_weather(NormalWeather const type, bool const is_extended) {
		activate_weather(type, BOUNDED_CONDITIONAL(is_extended, extended, standard));
	}
	constexpr auto activate_weather_from_ability(Generation const generation, NormalWeather const type, bool const is_extended) {
		auto const duration =
			BOUNDED_CONDITIONAL(generation <= Generation::five, permanent,
			BOUNDED_CONDITIONAL(is_extended, extended,
			standard
		));
		activate_weather(type, duration);
	}

	Short m_trick_room_turns_remaining = 0_bi;
	Short m_gravity_turns_remaining = 0_bi;
	Short m_magic_room_turns_remaining = 0_bi;
	NormalWeather m_active = NormalWeather::clear;
	Long m_turns_remaining = 0_bi;

	constexpr auto is_active(NormalWeather const type, bool const blocked_by_ability) const {
		return m_active == type and !blocked_by_ability;
	}
	
public:
	constexpr auto trick_room() const {
		return m_trick_room_turns_remaining != 0_bi;
	}
	constexpr auto gravity() const {
		return m_gravity_turns_remaining != 0_bi;
	}
	constexpr auto magic_room() const {
		return m_magic_room_turns_remaining != 0_bi;
	}
	constexpr auto hail(bool const blocked_by_ability) const {
		return is_active(NormalWeather::hail, blocked_by_ability);
	}
	constexpr auto sand(bool const blocked_by_ability) const {
		return is_active(NormalWeather::sand, blocked_by_ability);
	}
	constexpr auto sun(bool const blocked_by_ability) const {
		return is_active(NormalWeather::sun, blocked_by_ability);
	}
	constexpr auto rain(bool const blocked_by_ability) const {
		return is_active(NormalWeather::rain, blocked_by_ability);
	}

	constexpr auto advance_one_turn() {
		constexpr auto conditional_decrement = [](auto & n) {
			if (n > 0_bi) {
				--n;
			}
		};
		conditional_decrement(m_trick_room_turns_remaining);
		conditional_decrement(m_gravity_turns_remaining);
		conditional_decrement(m_magic_room_turns_remaining);
		conditional_decrement(m_turns_remaining);
		if (m_turns_remaining == 0_bi) {
			m_active = NormalWeather::clear;
		}
	}


	constexpr auto activate_trick_room() {
		m_trick_room_turns_remaining = BOUNDED_CONDITIONAL(trick_room(), 0_bi, standard);
	}

	constexpr auto activate_gravity() {
		if (!gravity()) {
			m_gravity_turns_remaining = standard;
		}
	}
	
	constexpr auto activate_magic_room() {
		m_magic_room_turns_remaining = BOUNDED_CONDITIONAL(magic_room(), 0_bi, standard);
	}
	
	constexpr auto deactivate_fog() {
	}

	constexpr auto activate_hail_from_move(bool const is_extended) {
		activate_weather(NormalWeather::hail, is_extended);
	}
	constexpr auto activate_hail_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, NormalWeather::hail, is_extended);
	}
	constexpr auto activate_sun_from_move(bool const is_extended) {
		activate_weather(NormalWeather::sun, is_extended);
	}
	constexpr auto activate_sun_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, NormalWeather::sun, is_extended);
	}
	constexpr auto activate_sand_from_move(bool const is_extended) {
		activate_weather(NormalWeather::sand, is_extended);
	}
	constexpr auto activate_sand_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, NormalWeather::sand, is_extended);
	}
	constexpr auto activate_rain_from_move(bool const is_extended) {
		activate_weather(NormalWeather::rain, is_extended);
	}
	constexpr auto activate_rain_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, NormalWeather::rain, is_extended);
	}

	friend constexpr auto operator==(Weather const lhs, Weather const rhs) {
		return
			lhs.m_trick_room_turns_remaining == rhs.m_trick_room_turns_remaining and
			lhs.m_gravity_turns_remaining == rhs.m_gravity_turns_remaining and
			lhs.m_magic_room_turns_remaining == rhs.m_magic_room_turns_remaining and
			lhs.m_active == rhs.m_active and
			lhs.m_turns_remaining == rhs.m_turns_remaining;
	}
};

}	// namespace technicalmachine
