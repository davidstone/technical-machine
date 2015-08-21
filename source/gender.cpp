// Gender functions
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

#include "gender.hpp"

namespace technicalmachine {
using namespace bounded::literal;

Gender::Gender (Genders gender_):
	gender (gender_) {
}

bounded::integer<-1, 1> multiplier(Gender const my, Gender const foe) {
	return (static_cast<bounded::integer<0, 2>>(my.gender) - 1_bi) * (static_cast<bounded::integer<0, 2>>(foe.gender) - 1_bi);
}

bool operator== (Gender const lhs, Gender const rhs) {
	return lhs.gender == rhs.gender;
}

bool operator!= (Gender const lhs, Gender const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
