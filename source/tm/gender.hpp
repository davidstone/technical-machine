// Gender header
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

namespace technicalmachine {

// TODO: Get rid of the intermediate struct

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
