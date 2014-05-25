// Class that handles Magnet Rise
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

#ifndef Magnet_RISE_HPP_
#define Magnet_RISE_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include <cstdint>

namespace technicalmachine {

class MagnetRise {
private:
	using duration_type = bounded::integer<0, 5>;
public:
	MagnetRise();
	bool is_active() const;
	void activate();
	void decrement();
	duration_type turns_remaining() const;
	typedef uint64_t hash_type;
	hash_type hash() const;
	static hash_type max_hash();
private:
	bounded::equivalent_type<duration_type, bounded::clamp_policy> m_turns_remaining;
};

bool operator== (MagnetRise const & lhs, MagnetRise const & rhs);
bool operator!= (MagnetRise const & lhs, MagnetRise const & rhs);

}	// namespace technicalmachine
#endif	// Magnet_RISE_HPP_
