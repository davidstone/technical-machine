// Gender functions
// Copyright (C) 2012 David Stone
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

#include "gender.hpp"
#include "string_conversions/gender_string.hpp"

namespace technicalmachine {

Gender::Gender ():
	gender (MALE)		// No sexism here!
	{
}

Gender::Gender (Genders gender_):
	gender (gender_) {
}

int Gender::multiplier (Gender foe) const {
	// Return 1 for same gender, -1 for opposite, and 0 if either is genderless
	return (gender - 1) * (foe.gender - 1);
}

std::string Gender::to_string() const {
	return ::technicalmachine::to_string(gender);
}

bool operator== (Gender const lhs, Gender const rhs) {
	return lhs.gender == rhs.gender;
}

bool operator!= (Gender const lhs, Gender const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
