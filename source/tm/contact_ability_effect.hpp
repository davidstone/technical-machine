// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <cstdint>

namespace technicalmachine {

enum class ContactAbilityEffect : std::uint8_t {
	nothing,
	burn,
	infatuation,
	paralysis,
	poison,
	sleep
};

} // namespace technicalmachine
