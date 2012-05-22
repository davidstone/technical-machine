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
#include <utility>

#include "../variable.hpp"
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
		static std::string to_string (Moves name);
		std::string to_string () const;
		static Moves from_string (std::string const & str);
		bool operator== (Move const & other) const;
		bool operator!= (Move const & other) const;
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
		bool was_used_last () const;
		bool cannot_ko () const;
		bool is_out_of_pp () const;
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
		int16_t score;
		Moves name;
		// I maintain the selectable state to determine if Struggle is legal
		bool selectable;
		uint8_t accuracy;		// A number between 0 (1?) and 100, according to poccil.
		uint8_t disable;			// Number of turns left on this move being Disabled (4-7)
		uint8_t pp_max;			// PP after all PP ups are applied
		uint8_t pp;
		// Move both of these up to team when it will reduce the size of Move.
		uint8_t r;					// The random number (85 through 100)
		uint8_t times_used;
	private:
		bool affects_pokemon (Team const & target, Pokemon const & pokemon, Weather const & weather) const;

		// Hidden Power makes this hard to replace with just a function
		uint8_t cached_base_power;
		Type cached_type;

		// Replace this with a function when it will reduce the size of Move.
		int8_t cached_priority;

		// Replace this with a function when it will reduce the size of Move.
		enum Classification : int8_t { special = -1, neither = 0, physical = 1 };
		static Classification classification (Moves move);
		Classification cached_classification;
};

// Various states a Pokemon can be in due to vanishing moves.

enum Vanish : uint8_t {
	LANDED, BOUNCED, DUG, DIVED,
	FLOWN, SHADOW_FORCED, END_VANISH
};

}	// namespace technicalmachine
#endif	// MOVE__MOVE_HPP_
