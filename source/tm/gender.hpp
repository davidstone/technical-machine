// Gender header
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {

enum class Gender : uint8_t { female, genderless, male };

} // namespace technicalmachine
namespace bounded {

template<>
inline constexpr auto min_value<technicalmachine::Gender> = technicalmachine::Gender();

template<>
inline constexpr auto max_value<technicalmachine::Gender> = technicalmachine::Gender::male;

}	// namespace bounded
namespace technicalmachine {

using namespace bounded::literal;

constexpr bounded::integer<-1, 1> multiplier(Gender const my, Gender const foe) {
	if (my == Gender::genderless or foe == Gender::genderless) {
		return 0_bi;
	} else if (my == foe) {
		return 1_bi;
	} else {
		return -1_bi;
	}
}

}	// namespace technicalmachine
