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

#include "base_power.hpp"
#include "moves_forward.hpp"
#include "pp.hpp"
#include "times_used.hpp"

#include "../type/type.hpp"

namespace technicalmachine {
class ActivePokemon;
class Weather;

class Move {
public:
	Move (Moves move, unsigned pp_ups = 3);
	Moves name() const;
	operator Moves() const;
	void reset ();
	Type type() const;
	void set_type(Type::Types t);	// for Hidden Power only.
	unsigned base_power() const;
	void increment_use_counter();
	bool was_used_last () const;
	unsigned fury_cutter_power() const;
	unsigned momentum_move_power() const;
	unsigned triple_kick_power() const;
	Rational metronome_boost() const;
	typedef uint64_t hash_type;
	hash_type hash() const;
	hash_type max_hash() const;
	friend bool operator== (Move const & lhs, Move const & rhs);

private:
	Moves m_name;
public:
	Pp pp;
private:
	// Move times_used up to ActivePokemon when it will reduce the size of Move.
	TimesUsed times_used;
	// Hidden Power makes this hard to replace with just a function
	BasePower m_base_power;
	Type m_type;
};

bool operator!= (Move const & lhs, Move const & rhs);

bool is_physical(Moves move);
bool is_special(Moves move);

bool is_switch(Moves name);
bool is_damaging(Move const & move);
bool is_phaze(Moves name);

bool is_usable_while_frozen(Moves move);

Moves from_replacement(unsigned replacement);
unsigned to_replacement(Moves name);

}	// namespace technicalmachine
#endif	// MOVE__MOVE_HPP_
