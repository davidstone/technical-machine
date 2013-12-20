// Handles bide damage and when it activates
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

#ifndef BIDE__BIDE_HPP_
#define BIDE__BIDE_HPP_

#include <cstdint>

#include "damage.hpp"
#include "duration.hpp"

namespace technicalmachine {

class Bide {
public:
	bool is_active() const;
	void activate();
	void add_damage(bounded_integer::checked_integer<0, BideDamage::max_hp - 1> damage);
	void reset();
	// Returns the damage released by Bide (or 0 if none)
	bounded_integer::native_integer<0, BideDamage::max_hp> decrement();
	typedef uint64_t hash_type;
	hash_type hash() const;
	static hash_type max_hash();
	friend bool operator== (Bide lhs, Bide rhs);
private:
	BideDamage m_damage;
	BideDuration m_duration;
};

bool operator!= (Bide lhs, Bide rhs);

}	// namespace technicalmachine
#endif	// BIDE__BIDE_HPP_
