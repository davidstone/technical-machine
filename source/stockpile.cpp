// Class that handles Stockpile
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

#include "stockpile.hpp"
#include "invalid_swallow.hpp"
#include "rational.hpp"

namespace technicalmachine {
namespace {
constexpr unsigned max_level = 3;
}	// unnamed namespace

Stockpile::Stockpile() :
	level(0)
	{
}

bool Stockpile::increment() {
	if (level == max_level)
		return false;

	++level;
	return true;
}

int Stockpile::release() {
	int const temp = level;
	level = 0;
	return temp;
}

void Stockpile::reset() {
	level = 0;
}

unsigned Stockpile::spit_up_power() const {
	return level * 100u;
}

Rational Stockpile::swallow_healing(int const stockpiles) {
	switch (stockpiles) {
		case 1:
			return Rational(1, 4);
		case 2:
			return Rational(1, 2);
		case 3:
			return Rational(1, 1);
		default:
			// A level of 0 means the move fails, so I'm disallowing it.
			throw InvalidSwallow(stockpiles);
	}
}

Stockpile::hash_type Stockpile::hash() const {
	return level;
}

Stockpile::hash_type Stockpile::max_hash() {
	return max_level + 1;
}

bool operator== (Stockpile const & lhs, Stockpile const & rhs) {
	return lhs.level == rhs.level;
}

bool operator!= (Stockpile const & lhs, Stockpile const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
