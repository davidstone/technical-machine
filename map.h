// Map enum to string
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MAP_H_
#define MAP_H_

#include <map>
#include <string>
#include "ability.h"
#include "gender.h"
#include "item.h"
#include "move.h"
#include "stat.h"
#include "species.h"

namespace technicalmachine {

class Map {
	public:
		std::map <std::string, abilities> ability;
		std::map <std::string, genders> gender;
		std::map <std::string, items> item;
		std::map <std::string, moves_list> move;
		std::map <std::string, natures> nature;
		std::map <std::string, species> specie;
	
		Map ();

	private:
		void set_ability ();
		void set_gender ();
		void set_item ();
		void set_move ();
		void set_nature ();
		void set_species ();
};

}

#endif
