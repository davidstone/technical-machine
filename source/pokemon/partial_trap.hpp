// Partial trap timer (things like Wrap and Clamp)
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

#ifndef PARTIAL_TRAP_HPP_
#define PARTIAL_TRAP_HPP_

#include <cstdint>

namespace technicalmachine {
class ActivePokemon;
class Pokemon;

// This class handles the number of turns remaining on Bind, Clamp, Fire Spin,
// Magma Storm, Sand Tomb, Whirlpool, and Wrap
class PartialTrap {
public:
	PartialTrap();
	void activate(bool extended = false);
	void damage(ActivePokemon & pokemon);
	void reset();
	explicit operator bool() const;
	friend bool operator== (PartialTrap const & lhs, PartialTrap const & rhs);
	typedef uint64_t hash_type;
	hash_type hash () const;
	static hash_type max_hash();
private:
	friend class Evaluate;
	bool is_active() const;
	uint8_t turns_active;
};

bool operator!= (PartialTrap const & lhs, PartialTrap const & rhs);

}	// namespace technicalmachine
#endif	// PARTIAL_TRAP_HPP_
