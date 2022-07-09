// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <numeric_traits/min_max_value.hpp>

#include <cstdint>

namespace technicalmachine {

enum class StatusName : std::uint8_t {
	clear,
	burn,
	freeze,
	paralysis,
	poison,
	toxic,
	sleep,
	rest
};

} // namespace technicalmachine

template<>
inline constexpr auto numeric_traits::min_value<technicalmachine::StatusName> = technicalmachine::StatusName();

template<>
inline constexpr auto numeric_traits::max_value<technicalmachine::StatusName> = technicalmachine::StatusName::rest;
