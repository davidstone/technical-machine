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

#ifndef TAUNT_HPP_
#define TAUNT_HPP_

#include <cstdint>

namespace technicalmachine {

class Taunt {
public:
	Taunt();
	bool is_active() const;
	void activate();
	void increment();
	void reset();
	friend bool operator== (Taunt const & lhs, Taunt const & rhs);
	typedef uint64_t hash_type;
	hash_type hash() const;
	static hash_type max_hash();
private:
	friend class Evaluate;
	uint8_t turns_taunted;
};
bool operator!= (Taunt const & lhs, Taunt const & rhs);

}	// namespace technicalmachine
#endif	// TAUNT_HPP_
