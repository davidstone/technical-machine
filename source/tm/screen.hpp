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

#include <tm/compress.hpp>
#include <tm/exists_if.hpp>
#include <tm/generation.hpp>
#include <tm/operators.hpp>

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
			--m_turns_remaining;
		}
	}
	
	friend auto operator==(Screen const &, Screen const &) -> bool = default;
	friend constexpr auto compress(Screen const value) {
		return compress(value.m_turns_remaining);
	}
private:
	using duration_type = bounded::clamped_integer<0, max_duration>;
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

}	// namespace technicalmachine
