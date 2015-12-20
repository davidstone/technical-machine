// Gender header
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

#include "enum.hpp"

#include <bounded/integer.hpp>

#include <cstdint>

namespace technicalmachine {

// TODO: Get rid of the intermediate struct

struct Gender {
	enum Genders : uint8_t { FEMALE, GENDERLESS, MALE, END };
	
	Gender(Genders gender_);

	friend bool operator== (Gender lhs, Gender rhs);

	Genders gender;
};

// Return 1 if the same, -1 if opposite, and 0 if either is genderless
bounded::integer<-1, 1> multiplier(Gender me, Gender foe);

}	// namespace technicalmachine

namespace std {

template<>
struct numeric_limits<technicalmachine::Gender::Genders> : technicalmachine::enum_numeric_limits<technicalmachine::Gender::Genders> {};

}	// namespace std
