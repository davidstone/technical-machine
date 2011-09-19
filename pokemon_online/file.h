// Load Pokemon Online teams
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_ONLINE_FILE_H_
#define POKEMON_ONLINE_FILE_H_

#include <fstream>
#include <string>
#include "../ability.h"
#include "../item.h"
#include "../move.h"
#include "../species.h"
#include "../stat.h"


namespace technicalmachine {

class Team;

namespace po {

void load_team (Team & team, std::string const & name, unsigned size);
void load_pokemon (Team & team, std::ifstream & file, species const pokemon_converter [], Ability::Abilities const ability_converter [], Item::Items const item_converter [], Move::moves_list const move_converter [], unsigned size);
unsigned converter (std::string const & data, std::string const & end, std::string const & line);

}

}

#endif
