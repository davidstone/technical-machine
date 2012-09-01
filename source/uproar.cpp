// Class to represent Uproar's counter
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

#include "uproar.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned min_guarantee = 2;
constexpr unsigned max = 5;
}	// unnamed namespace

Uproar::Uproar() :
	counter(0)
	{
}

void Uproar::reset() {
	counter = 0;
}

void Uproar::increment() {
	if (counter != max)
		++counter;
	else
		reset();
}

Uproar::hash_type Uproar::hash() const {
	return counter;
}

Uproar::hash_type Uproar::max_hash() {
	// I think this is correct
	return max;
}

bool operator== (Uproar const & lhs, Uproar const & rhs) {
	return lhs.counter == rhs.counter;
}

bool operator!= (Uproar const & lhs, Uproar const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
