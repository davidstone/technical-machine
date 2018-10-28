// Utility to make enum -> bounded::integer conversion easier
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

#include <bounded/integer.hpp>

#include <cstdint>
#include <functional>
#include <type_traits>

namespace technicalmachine {

BOUNDED_COMPARISON
BOUNDED_COMMON_ARITHMETIC

using namespace bounded::literal;

template<auto maximum>
struct enum_numeric_limits : private std::numeric_limits<bounded::integer<0, static_cast<std::intmax_t>(maximum)>> {
private:
	using base = std::numeric_limits<bounded::integer<0, static_cast<std::intmax_t>(maximum)>>;
public:
	static constexpr bool is_specialized = true;
	static constexpr bool is_signed = false;
	static constexpr bool is_integer = false;
	static constexpr bool is_exact = true;
	static constexpr bool has_infinity = false;
	static constexpr bool has_quiet_NaN = false;
	static constexpr bool has_signaling_NaN = false;
	static constexpr auto has_denorm = std::denorm_absent;
	static constexpr bool has_denorm_loss = false;
	static constexpr auto round_style = base::round_style;
	static constexpr bool is_iec559 = false;
	static constexpr bool is_bounded = true;
	static constexpr bool is_modulo = false;
	static constexpr auto radix = base::radix;
	static constexpr auto digits = base::digits;
	static constexpr auto digits10 = base::digits10;
	static constexpr auto max_digits10 = base::max_digits10;
	static constexpr auto min_exponent = base::min_exponent;
	static constexpr auto min_exponent10 = base::min_exponent10;
	static constexpr auto max_exponent = base::max_exponent;
	static constexpr auto max_exponent10 = base::max_exponent10;

	// If 0 is not in range, there is no trap value for arithmetic
	static constexpr auto traps = base::traps;

	static constexpr bool tinyness_before = base::tinyness_before;

	using base::min;
	using base::lowest;
	using base::max;
	using base::epsilon;
	using base::round_error;
	using base::infinity;
	using base::quiet_NaN;
	using base::signaling_NaN;
	using base::denorm_min;
};

template<typename Enum>
struct std_hash {
	constexpr auto operator()(Enum const e) const {
		using T = std::underlying_type_t<Enum>;
		return std::hash<T>{}(static_cast<T>(e));
	}
};

}	// namespace technicalmachine
