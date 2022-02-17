// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/compress.hpp>
#include <tm/exists_if.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>
#include <tm/saturating_add.hpp>

#include <bounded/integer.hpp>
#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

template<bool exists, int normal_duration, int max_duration = normal_duration>
struct Screen {
	static_assert(normal_duration <= max_duration, "Max duration cannot be less than normal duration.");

	constexpr explicit operator bool() const {
		return m_turns_remaining != 0_bi;
	}
	constexpr auto turns_remaining() const {
		return m_turns_remaining;
	}
	constexpr auto activate() & -> void requires(normal_duration == max_duration) {
		set(bounded::constant<normal_duration>);
	}
	constexpr auto activate(bool const is_extended) & -> void requires(normal_duration != max_duration) {
		set(BOUNDED_CONDITIONAL(is_extended,
			bounded::constant<max_duration>,
			bounded::constant<normal_duration>
		));
	}
	constexpr auto decrement() & -> void {
		if constexpr (exists) {
			saturating_decrement(m_turns_remaining);
		}
	}

	friend auto operator==(Screen, Screen) -> bool = default;
	friend constexpr auto compress(Screen const value) {
		return compress(value.m_turns_remaining);
	}

private:
	using duration_type = bounded::integer<0, max_duration>;
	constexpr auto set(duration_type const duration) & -> void {
		if constexpr (exists) {
			if (m_turns_remaining == 0_bi) {
				m_turns_remaining = duration;
			}
		}
	}
	[[no_unique_address]] IntegerIf<duration_type, exists> m_turns_remaining = 0_bi;
};

template<Generation generation>
using LuckyChant = Screen<generation >= Generation::four, 5>;

using Mist = Screen<true, 5>;

template<Generation generation>
using Safeguard = Screen<generation >= Generation::two, 5>;

using Reflect = Screen<true, 5, 8>;

using LightScreen = Screen<true, 5, 8>;

template<Generation generation>
using Tailwind = Screen<generation >= Generation::four, 3>;

} // namespace technicalmachine
