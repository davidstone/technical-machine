// Pokemon data structure
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_H_
#define POKEMON_H_

#include <string>
#include <vector>

#include "ability.h"
#include "active.h"
#include "gender.h"
#include "item.h"
#include "species.h"
#include "stat.h"
#include "status.h"
#include "type.h"

namespace technicalmachine {

class Ability;
class Move;

class Pokemon {
	public:
		std::string nickname;
		std::vector <Type> type;
		Species name;
		Ability ability;
		Gender gender;
		Item item;
		Status status;
		Active<Move> move;
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
		int8_t mass;		// mass == power of Grass Knot / Low Kick, not the actual mass stat
		int8_t sleep;				// wakes up when this number is 2 through 5

		uint8_t happiness;
	
		Pokemon (Species const &member, unsigned size);
		uint64_t hash () const;
		std::string get_name () const;
		static Species name_from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
		bool operator== (Pokemon const & other) const;
		bool operator!= (Pokemon const & other) const;
		void load ();
	private:
		void set_type ();
		int8_t get_mass () const;
};

}
#endif		// POKEMON_H_
