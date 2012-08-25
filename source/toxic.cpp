// Class to represent Toxic's counter
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

#include "toxic.hpp"
#include "rational.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max = 15;
}	// unnamed namespace

Toxic::Toxic() :
	counter(0)
	{
}

void Toxic::reset() {
	counter = 0;
}

void Toxic::increment() {
	if (counter != max)
		++counter;
}

Rational Toxic::ratio_drained() const {
	return Rational(counter, 16);
}

Toxic::hash_type Toxic::hash() const {
	return counter;
}

Toxic::hash_type Toxic::max_hash() {
	return max + 1;
}

bool operator== (Toxic const & lhs, Toxic const & rhs) {
	return lhs.counter == rhs.counter;
}

bool operator!= (Toxic const & lhs, Toxic const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
