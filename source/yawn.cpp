// Class to represent Yawn's counter
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

#include "yawn.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned initial_value = 2;
}	// unnamed namespace

Yawn::Yawn() :
	counter(0)
	{
}

void Yawn::reset() {
	counter = 0;
}

bool Yawn::is_inactive() const {
	return counter == 0;
}

void Yawn::activate() {
	if (is_inactive())
		counter = initial_value;
}

bool Yawn::decrement() {
	if (is_inactive())
		return false;
	--counter;
	return is_inactive();
}

Yawn::hash_type Yawn::hash() const {
	return counter;
}

Yawn::hash_type Yawn::max_hash(){
	// It is decremented at the end of the turn, and hashes are calculated
	// after the end of turn.
	return initial_value;
}

bool operator== (Yawn const & lhs, Yawn const & rhs) {
	return lhs.counter == rhs.counter;
}

bool operator!= (Yawn const & lhs, Yawn const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
