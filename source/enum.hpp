// Utility to make enum -> bounded::integer conversion easier
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

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <type_traits>

namespace technicalmachine {

template<typename Enum>
struct basic_numeric_limits {
	static constexpr auto min() noexcept -> std::intmax_t {
		return 0;
	}
	static constexpr auto max() noexcept -> std::intmax_t {
		return static_cast<std::intmax_t>(Enum::END);
	}
	static constexpr bool is_specialized = true;
	static constexpr bool is_integer = false;
};

template<typename Enum>
constexpr auto hash(Enum const e) noexcept {
	static_assert(std::is_enum<Enum>::value, "Only usable with enum types.");
	return hash(bounded::make(e));
}

template<typename Enum>
struct std_hash {
	constexpr auto operator()(Enum const e) const {
		using T = std::underlying_type_t<Enum>;
		return std::hash<T>{}(static_cast<T>(e));
	}
};

template<typename Enum>
struct InvalidEnum : std::runtime_error {
	static_assert(std::is_enum<Enum>::value, "Only valid for enums.");
	explicit InvalidEnum(Enum const e):
		std::runtime_error(std::to_string(static_cast<std::underlying_type_t<Enum>>(e))) {
	}
};

}	// namespace technicalmachine
