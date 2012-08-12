// Wish data structure
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

#include "wish.hpp"
#include "heal.hpp"

namespace technicalmachine {
class Pokemon;

Wish::Wish() :
	counter(0) {
}

void Wish::activate() {
	if (!is_active())
		counter = 2;
}

void Wish::decrement(Pokemon & pokemon) {
	if (is_active())
		--counter;
	heal(pokemon, 2);
}

bool Wish::is_active() const {
	return counter != 0;
}

Wish::hash_type Wish::hash() const {
	return counter;
}

Wish::hash_type Wish::max_hash() {
	return 2;
}

// 2 = Wish will heal next turn, 1 = Wish will heal this turn, 0 = Wish
// is not in play

bool operator== (Wish const lhs, Wish const rhs) {
	return lhs.counter == rhs.counter;
}
bool operator!= (Wish const lhs, Wish const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
