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

#include <cstdint>

#include "classification.hpp"
#include "disable.hpp"
#include "moves_forward.hpp"
#include "pp.hpp"
#include "priority.hpp"
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
	bool affects_target (ActivePokemon const & target, Weather const & weather) const;
	bool has_follow_up_decision () const;
	bool calls_other_move () const;
	void increment_use_counter();
	bool was_used_last () const;
	bool is_bide() const;
	unsigned fury_cutter_power() const;
	unsigned momentum_move_power() const;
	unsigned triple_kick_power() const;
	Rational metronome_boost() const;
	bool cannot_ko () const;
	bool breaks_screens () const;
	static bool is_phaze (Moves name);
	bool is_phaze () const;
	static bool is_healing (Moves name);
	bool is_healing () const;
	bool is_boosted_by_iron_fist () const;
	bool is_boosted_by_reckless() const;
	bool is_usable_while_sleeping () const;
	bool is_usable_while_frozen () const;
	bool is_sound_based () const;
	bool is_self_KO () const;
	void get_magnitude (unsigned magnitude);
	static constexpr unsigned max_regular_moves () {
		return 4;
	}
	typedef uint64_t hash_type;
	hash_type hash() const;
	hash_type max_hash() const;
	friend bool operator== (Move const & lhs, Move const & rhs);

private:
	Moves m_name;
public:
	Pp pp;
	Disable disable;
private:
	// Move times_used up to ActivePokemon when it will reduce the size of Move.
	TimesUsed times_used;
	// Hidden Power makes this hard to replace with just a function
	uint8_t cached_base_power;
	Type cached_type;
};

bool operator!= (Move const & lhs, Move const & rhs);

bool is_physical(Moves move);
bool is_special(Moves move);

bool is_regular(Move const & move);
bool is_struggle(Move const & move);
bool is_switch(Moves name);
bool is_damaging(Move const & move);

bool is_blocked_by_taunt(Move const & move);
bool is_blocked_by_gravity(Move const & move);

bool can_critical_hit(Move const & move);

Moves from_replacement(unsigned replacement);
unsigned to_replacement(Moves name);


}	// namespace technicalmachine
#endif	// MOVE__MOVE_HPP_
