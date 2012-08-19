// Taunt class
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

#include "taunt.hpp"

// turns_taunted represents the number of turns the Pokemon will have been under
// the effects of Taunt at the end of the turn. After at most max_taunt_length
// turns, the effect ends.

namespace technicalmachine {
namespace {
constexpr unsigned min_taunt_length = 2;
constexpr unsigned max_taunt_length = 3;
}	// unnamed namespace

Taunt::Taunt() :
	turns_taunted(0)
	{
}

bool Taunt::is_active() const {
	return turns_taunted != 0;
}

void Taunt::activate() {
	if (turns_taunted == 0)
		turns_taunted = 1;
}

void Taunt::increment() {
	// TODO: handle Taunt that ends after two turns
	if (turns_taunted == max_taunt_length)
		turns_taunted = 0;
	else if (turns_taunted != 0)
		++turns_taunted;
}

void Taunt::reset() {
	turns_taunted = 0;
}

Taunt::hash_type Taunt::hash() const {
	return turns_taunted;
}

Taunt::hash_type Taunt::max_hash() {
	// Taunt is cleared at the end of the turn, and hashes are calculated after
	// the end of the turn. Therefore, we don't need + 1.
	return max_taunt_length;
}

bool operator== (Taunt const & lhs, Taunt const & rhs) {
	return lhs.turns_taunted == rhs.turns_taunted;
}

bool operator!= (Taunt const & lhs, Taunt const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
