// Class to represent Toxic's counter
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

#ifndef TOXIC_HPP_
#define TOXIC_HPP_

#include "../rational.hpp"
#include <bounded_integer/bounded_integer.hpp>
#include <cstdint>

namespace technicalmachine {
using namespace bounded::literal;
class Rational;

class Toxic {
public:
	Toxic() = default;
	void increment();
	auto ratio_drained() const {
		return make_bounded_rational(m_counter, 16_bi);
	}
	friend bool operator== (Toxic const & lhs, Toxic const & rhs);
	typedef uint64_t hash_type;
	hash_type hash() const;
	static hash_type max_hash();
private:
	friend class Evaluate;
	// Number of turns this Pokemon has already taken Toxic damage (or
	// would have if Magic Guard / Poison Heal weren't in play)
	bounded::clamped_integer<0, 15> m_counter = 0_bi;
};
bool operator!= (Toxic const & lhs, Toxic const & rhs);

}	// namespace technicalmachine
#endif	// TOXIC_HPP_
