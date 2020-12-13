// Base power calculation
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>
#include <bounded/optional.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;

// variable power returns non-0. Fixed damage is the uninitialized state.
auto base_power(Generation generation, Moves move) -> bounded::optional<bounded::integer<0, 250>>;

// Fling gives 0, Rollout gives 480
using VariableAdjustedBasePower = bounded::integer<0, 480>;

}	// namespace technicalmachine
