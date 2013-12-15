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
#include "../rational.hpp"

namespace technicalmachine {

Stockpile::Stockpile() :
	m_level(0_bi)
	{
}

bool Stockpile::increment() {
	auto const initial = m_level;
	++m_level;
	return m_level == initial;
}

auto Stockpile::release() -> bounded_integer::native_integer<0, max> {
	auto const temp = m_level;
	reset();
	return temp;
}

void Stockpile::reset() {
	m_level = 0_bi;
}

auto Stockpile::spit_up_power() const -> bounded_integer::native_integer<0, max * 100> {
	return m_level * 100_bi;
}


Rational swallow_healing(bounded_integer::checked_integer<1, Stockpile::max> const stockpiles) {
	switch (stockpiles.value()) {
		case 1:
			return Rational(1, 4);
		case 2:
			return Rational(1, 2);
		default:	// case 3:
			return Rational(1, 1);
	}
}

Stockpile::hash_type Stockpile::hash() const {
	return static_cast<hash_type>(m_level);
}

Stockpile::hash_type Stockpile::max_hash() {
	return max + 1;
}

bool operator== (Stockpile const & lhs, Stockpile const & rhs) {
	return lhs.m_level == rhs.m_level;
}

bool operator!= (Stockpile const & lhs, Stockpile const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
