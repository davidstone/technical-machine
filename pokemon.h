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
#include "move.h"
#include "species.h"
#include "stat.h"
#include "status.h"
#include "type.h"

namespace technicalmachine {

class Pokemon {
	public:
		std::string nickname;
		species name;
		abilities ability;
		genders gender;
		items item;
		statuses status;
		types type1;
		types type2;
		Active<Move> move;
		Stat hp;
		Stat atk;
		Stat def;
		Stat spa;
		Stat spd;
		Stat spe;
		natures nature;
		bool fainted;
		int8_t level;
		int8_t mass;		// mass == power of Grass Knot / Low Kick, not the actual mass stat
		int8_t sleep;				// wakes up when this number is 2 through 5

		uint8_t happiness;
	
		Pokemon (species const &member, unsigned size);
		uint64_t hash () const;
		std::string get_name () const;
		bool operator== (Pokemon const &other) const;
		bool operator!= (Pokemon const &other) const;
		void load ();
	private:
		static types const get_type [][2];
		static int8_t const get_mass [];
		static std::string const name_to_string [];
};

}
#endif
