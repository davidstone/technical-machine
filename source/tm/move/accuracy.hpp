// Accuracy functions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/optional.hpp>
#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
struct Weather;

// A value that is not present indicates that the move cannot miss.
using BaseAccuracy = bounded::optional<bounded::integer<30, 100>>;
auto accuracy(Generation generation, Moves move, Weather const weather, bool weather_blocked, bool user_is_poison) -> BaseAccuracy;

}	// namespace technicalmachine
