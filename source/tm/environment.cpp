// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/conditional.hpp>

export module tm.environment;

import tm.ability;
import tm.compress;
import tm.generation;
import tm.weather;

import bounded;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export struct Environment {
private:
	static constexpr auto standard = 5_bi;
	static constexpr auto extended = 8_bi;
	static constexpr auto permanent = -1_bi;
	using Short = bounded::integer<0, bounded::normalize<standard>>;
	using Long = bounded::integer<-1, bounded::normalize<extended>>;

	constexpr auto activate_weather(Weather const type, Long const duration) {
		if (type == m_active and duration != permanent) {
			return;
		}
		m_active = type;
		m_turns_remaining = static_cast<Long>(duration);
	}
	constexpr auto activate_weather(Weather const type, bool const is_extended) {
		activate_weather(type, BOUNDED_CONDITIONAL(is_extended, extended, standard));
	}
	constexpr auto activate_weather_from_ability(Generation const generation, Weather const type, bool const is_extended) {
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
	Weather m_active = Weather::clear;
	Long m_turns_remaining = 0_bi;

	constexpr auto is_active(Weather const type) const {
		return m_active == type;
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
	constexpr auto hail() const {
		return is_active(Weather::hail);
	}
	constexpr auto sand() const {
		return is_active(Weather::sand);
	}
	constexpr auto sun() const {
		return is_active(Weather::sun);
	}
	constexpr auto rain() const {
		return is_active(Weather::rain);
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
			m_active = Weather::clear;
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
		activate_weather(Weather::hail, is_extended);
	}
	constexpr auto activate_hail_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, Weather::hail, is_extended);
	}
	constexpr auto activate_sun_from_move(bool const is_extended) {
		activate_weather(Weather::sun, is_extended);
	}
	constexpr auto activate_sun_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, Weather::sun, is_extended);
	}
	constexpr auto activate_sand_from_move(bool const is_extended) {
		activate_weather(Weather::sand, is_extended);
	}
	constexpr auto activate_sand_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, Weather::sand, is_extended);
	}
	constexpr auto activate_rain_from_move(bool const is_extended) {
		activate_weather(Weather::rain, is_extended);
	}
	constexpr auto activate_rain_from_ability(Generation const generation, bool const is_extended) {
		activate_weather_from_ability(generation, Weather::rain, is_extended);
	}

	friend auto operator==(Environment, Environment) -> bool = default;

	friend constexpr auto compress(Environment const environment) {
		return compress_combine(
			environment.m_trick_room_turns_remaining,
			environment.m_gravity_turns_remaining,
			environment.m_magic_room_turns_remaining,
			environment.m_active,
			environment.m_turns_remaining
		);
	}
};

export constexpr auto to_weather(Environment const environment) -> Weather {
	if (environment.hail()) {
		return Weather::hail;
	} else if (environment.sand()) {
		return Weather::sand;
	} else if (environment.sun()) {
		return Weather::sun;
	} else if (environment.rain()) {
		return Weather::rain;
	} else {
		return Weather::clear;
	}
}

} // namespace technicalmachine
