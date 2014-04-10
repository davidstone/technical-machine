// Gender header
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

#ifndef GENDER_HPP_
#define GENDER_HPP_

#include <bounded_integer/bounded_integer.hpp>

#include <cstdint>

namespace technicalmachine {

class Gender {
public:
	enum Genders : uint8_t { FEMALE, GENDERLESS, MALE, END };
	
	Gender();
	explicit Gender(Genders gender_);

	friend bool operator== (Gender lhs, Gender rhs);

	Genders gender;
};
bool operator!= (Gender lhs, Gender rhs);

// Return 1 if the same, -1 if opposite, and 0 if either is genderless
bounded::integer<-1, 1> multiplier(Gender me, Gender foe);

}	// namespace technicalmachine
#endif	// GENDER_HPP_
