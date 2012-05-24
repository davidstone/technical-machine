// Moves data structure
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

#ifndef MOVE__MOVE_HPP_
#define MOVE__MOVE_HPP_

#include <cstdint>
#include <string>

#include "classification.hpp"
#include "disable.hpp"
#include "pp.hpp"
#include "times_used.hpp"

#include "../rational.hpp"
#include "../variable_collection.hpp"
#include "../type.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
class Team;
class Weather;

class Move {
	public:
		Move (Moves move, unsigned pp_ups, unsigned size = 1);
		void reset ();
		uint64_t hash() const;
		uint64_t max_hash() const;
		static std::string to_string (Moves name);
		std::string to_string () const;
		static Moves from_string (std::string const & str);
		friend bool operator== (Move const & lhs, Move const & rhs);
		bool is_damaging() const;
		bool is_physical() const;
		bool is_special() const;
		Type type() const;
		void set_type(Type::Types t);	// for Hidden Power only.
		uint8_t base_power() const;
		bool can_critical_hit() const;
		int8_t priority() const;
		static bool is_switch (Moves name);
		bool is_switch () const;
		static Moves from_replacement (unsigned replacement);
		static unsigned to_replacement (Moves name);
		unsigned to_replacement () const;
		bool affects_target (Team const & target, Weather const & weather) const;
		bool affects_replacement (Team const & target, Weather const & weather) const;
		bool has_follow_up_decision () const;
		bool calls_other_move () const;
		void increment_use_counter();
		bool was_used_last () const;
		unsigned fury_cutter_power() const;
		unsigned momentum_move_power() const;
		unsigned triple_kick_power() const;
		Rational metronome_boost() const;
		bool cannot_ko () const;
		bool breaks_screens () const;
		bool is_struggle_or_switch () const;
		static bool is_phaze (Moves name);
		bool is_phaze () const;
		static bool is_healing (Moves name);
		bool is_healing () const;
		bool is_blocked_by_gravity () const;
		bool is_boosted_by_iron_fist () const;
		bool is_boosted_by_reckless() const;
		bool is_usable_while_sleeping () const;
		bool is_usable_while_frozen () const;
		bool is_sound_based () const;
		bool is_self_KO () const;
		bool cannot_miss () const;
		void get_magnitude (unsigned magnitude);
		static constexpr unsigned max_regular_moves () {
			return 4;
		}

		VariableCollection variable;
		Moves name;
		Pp pp;
		int16_t score;
		// I maintain the selectable state to determine if Struggle is legal
		bool selectable;
		// A number between 0 (1?) and 100, according to poccil.
		uint8_t accuracy;
		Disable disable;
		// Move r and times_used up to team when it will reduce the size of Move.
		// The random number (85 through 100)
		uint8_t r;
	private:
		TimesUsed times_used;
		bool affects_pokemon (Team const & target, Pokemon const & pokemon, Weather const & weather) const;
		// Hidden Power makes this hard to replace with just a function
		uint8_t cached_base_power;
		Type cached_type;
		// Replace this with a function when it will reduce the size of Move.
		int8_t cached_priority;
		// Replace this with a function when it will reduce the size of Move.
		Classification cached_classification;
};

bool operator!= (Move const & lhs, Move const & rhs);

}	// namespace technicalmachine
#endif	// MOVE__MOVE_HPP_
