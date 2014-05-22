// Screens
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

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include "hash.hpp"

#include <bounded_integer/bounded_integer.hpp>
#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;

template<intmax_t normal_duration, intmax_t max_duration = normal_duration>
class Screen {
public:
	static_assert(normal_duration <= max_duration, "Max duration cannot be less than normal duration.");

	constexpr operator bool() const {
		return m_turns_remaining != 0_bi;
	}
	constexpr auto turns_remaining() const {
		return m_turns_remaining;
	}
	template<typename ... Args>
	auto activate(Args && ... args) -> void {
		activate(std::integral_constant<bool, normal_duration == max_duration>{}, std::forward<Args>(args)...);
	}
	auto decrement() -> void {
		--m_turns_remaining;
	}
	
private:
	auto activate(std::true_type) -> void {
		set(bounded::make<normal_duration>());
	}
	auto activate(std::false_type, bool const is_extended) -> void {
		set(BOUNDED_CONDITIONAL(is_extended,
			bounded::make<max_duration>(),
			bounded::make<normal_duration>()
		));
	}
	using duration_type = bounded::clamped_integer<0, max_duration>;
	auto set(duration_type const duration) -> void {
		if (m_turns_remaining == 0_bi) {
			m_turns_remaining = duration;
		}
	}
	// Explicit call to constructor needed for workaround to gcc 4.9.0 bug.
	duration_type m_turns_remaining = duration_type(0_bi);
};

template<intmax_t normal_duration, intmax_t max_duration>
constexpr auto hash(Screen<normal_duration, max_duration> const screen) noexcept {
	return hash(screen.turns_remaining());
}
template<intmax_t normal_duration, intmax_t max_duration>
constexpr auto max_hash(Screen<normal_duration, max_duration>) noexcept {
	return bounded::make<max_duration>();
}

template<intmax_t normal_duration, intmax_t max_duration>
auto operator==(Screen<normal_duration, max_duration> const & lhs, Screen<normal_duration, max_duration> const & rhs) -> bool {
	return lhs.turns_remaining() == rhs.turns_remaining();
}
template<intmax_t normal_duration, intmax_t max_duration>
auto operator!=(Screen<normal_duration, max_duration> const & lhs, Screen<normal_duration, max_duration> const & rhs) -> bool {
	return !(lhs == rhs);
}

using LuckyChantEffect = Screen<5>;
using MistEffect = Screen<5>;
using SafeguardEffect = Screen<5>;
using ReflectEffect = Screen<5, 8>;
using LightScreenEffect = Screen<5, 8>;
using TailwindEffect = Screen<3>;

}	// namespace technicalmachine
#endif	// SCREEN_HPP_
