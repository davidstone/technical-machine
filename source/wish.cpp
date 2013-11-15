// Wish data structure
// Copyright (C) 2013 David Stone
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
#include "rational.hpp"

namespace technicalmachine {
class Pokemon;

void Wish::activate() {
	if (!is_active()) {
		static constexpr auto turn_delay = 1_ri;
		turns_until_activation = counter_type(turn_delay);
	}
}

void Wish::decrement(ActivePokemon & pokemon) {
	if (is_active()) {
		--*turns_until_activation;
		if (*turns_until_activation == 0_ri) {
			turns_until_activation = {};
			heal(pokemon, Rational(1, 2));
		}
	}
}

bool Wish::is_active() const {
	return static_cast<bool>(turns_until_activation);
}

Wish::hash_type Wish::hash() const {
	return is_active() ? 1 : 0;
}

Wish::hash_type Wish::max_hash() {
	return 2;
}

bool operator== (Wish const lhs, Wish const rhs) {
	return lhs.turns_until_activation == rhs.turns_until_activation;
}
bool operator!= (Wish const lhs, Wish const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
