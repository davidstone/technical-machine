// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>
#include <bounded/conditional.hpp>

export module tm.environment;

import tm.ability;
import tm.compress;
import tm.generation;
import tm.weather;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto ability_blocks_weather(Ability const ability) -> bool {
	switch (ability) {
		case Ability::Air_Lock:
		case Ability::Cloud_Nine:
			return true;
		default:
			return false;
	}
}

export struct Environment {
	constexpr auto trick_room() const -> bool {
		return m_trick_room_turns_remaining != 0_bi;
	}
	constexpr auto gravity() const -> bool {
		return m_gravity_turns_remaining != 0_bi;
	}
	constexpr auto magic_room() const -> bool {
		return m_magic_room_turns_remaining != 0_bi;
	}
	constexpr auto actual_weather() const -> Weather {
		return m_weather;
	}
	constexpr auto effective_weather(Ability const ability1, Ability const ability2) const -> Weather {
		return ability_blocks_weather(ability1) or ability_blocks_weather(ability2) ?
			Weather::clear :
			m_weather;
	}

	constexpr auto advance_one_turn() -> void {
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
			m_weather = Weather::clear;
		}
	}


	constexpr auto activate_trick_room() -> void {
		m_trick_room_turns_remaining = BOUNDED_CONDITIONAL(trick_room(), 0_bi, standard);
	}

	constexpr auto activate_gravity() -> void {
		if (!gravity()) {
			m_gravity_turns_remaining = standard;
		}
	}

	constexpr auto activate_magic_room() -> void {
		m_magic_room_turns_remaining = BOUNDED_CONDITIONAL(magic_room(), 0_bi, standard);
	}

	constexpr auto deactivate_fog() -> void {
	}

	constexpr auto activate_weather_from_move(Weather const weather, bool const is_extended) -> void {
		BOUNDED_ASSERT(weather != Weather::clear);
		activate_weather(weather, is_extended ? Long(extended) : Long(standard));
	}
	constexpr auto activate_weather_from_ability(Generation const generation, Weather const weather, bool const is_extended) -> void {
		BOUNDED_ASSERT(weather != Weather::clear);
		auto const duration =
			generation <= Generation::five ? Long(permanent) :
			is_extended ? Long(extended) :
			Long(standard);
		activate_weather(weather, duration);
	}

	friend auto operator==(Environment, Environment) -> bool = default;

	friend constexpr auto compress(Environment const environment) {
		return compress_combine(
			environment.m_trick_room_turns_remaining,
			environment.m_gravity_turns_remaining,
			environment.m_magic_room_turns_remaining,
			environment.m_weather,
			environment.m_turns_remaining
		);
	}

private:
	static constexpr auto standard = 5_bi;
	static constexpr auto extended = 8_bi;
	static constexpr auto permanent = -1_bi;
	using Short = bounded::integer<0, bounded::normalize<standard>>;
	using Long = bounded::integer<-1, bounded::normalize<extended>>;

	constexpr auto activate_weather(Weather const weather, Long const duration) -> void {
		if (m_weather == weather and duration != permanent) {
			return;
		}
		m_weather = weather;
		m_turns_remaining = duration;
	}

	Short m_trick_room_turns_remaining = 0_bi;
	Short m_gravity_turns_remaining = 0_bi;
	Short m_magic_room_turns_remaining = 0_bi;
	Weather m_weather = Weather::clear;
	Long m_turns_remaining = 0_bi;
};

} // namespace technicalmachine
