// Pokemon data structure
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

#ifndef POKEMON_HPP_
#define POKEMON_HPP_

#include <string>

#include "ability.hpp"
#include "active.hpp"
#include "gender.hpp"
#include "item.hpp"
#include "move.hpp"
#include "nature.hpp"
#include "species.hpp"
#include "stat.hpp"
#include "status.hpp"
#include "type.hpp"

namespace technicalmachine {

class Pokemon {
	public:
		std::string nickname;
		TypeCollection type;
		Species name;
		Ability ability;
		Gender gender;
		Item item;
		Active<Move> move;
		Status status;
		Stat hp;
		Stat atk;
		Stat def;
		Stat spa;
		Stat spd;
		Stat spe;
		Nature nature;
		int16_t new_hp;		// 0 through 48 for foes, used to keep the HP learned from the log on track with reality
		bool fainted;
		int8_t level;
		int8_t mass;		// power of Grass Knot / Low Kick, not the actual mass stat
		int8_t sleep;				// wakes up when this number is 2 through 5

		uint8_t happiness;
	
		Pokemon (Species const &member, unsigned size);
		uint64_t hash () const;
		// Returns whether the move is found. Modifies move.index
		bool find_move (Move::Moves name);
		void normalize_hp ();
		static std::string to_string (Species name);
		std::string to_string () const;
		static Species from_string (std::string const & str);
		static bool is_alternate_form (Species first, Species second);
		static bool is_deoxys (Species species);
		static bool is_giratina (Species species);
		static bool is_rotom (Species species);
		static bool is_shaymin (Species species);
		static bool is_wormadam (Species species);
		bool operator== (Pokemon const & other) const;
		bool operator!= (Pokemon const & other) const;
		void load ();
	private:
		void set_type ();
		int8_t get_mass () const;
};

}	// namespace technicalmachine
#endif	// POKEMON_HPP_
