// Moves data structure
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

#ifndef MOVE__MOVE_HPP_
#define MOVE__MOVE_HPP_

#include "moves_forward.hpp"
#include "pp.hpp"

namespace technicalmachine {

class Move {
public:
	explicit Move(Moves move, unsigned pp_ups = 3);
	operator Moves() const;
	typedef uint64_t hash_type;
	hash_type hash() const;
	hash_type max_hash() const;
	friend bool operator== (Move const & lhs, Move const & rhs);

private:
	Moves m_name;
public:
	Pp pp;
};

bool operator!= (Move const & lhs, Move const & rhs);

bool is_physical(Moves move);
bool is_special(Moves move);

bool is_switch(Moves name);
bool is_damaging(Moves move);
bool is_phaze(Moves name);

bool is_usable_while_frozen(Moves move);

Moves from_replacement(unsigned replacement);
unsigned to_replacement(Moves name);

}	// namespace technicalmachine
#endif	// MOVE__MOVE_HPP_
