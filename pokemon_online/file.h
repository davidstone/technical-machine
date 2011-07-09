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

void load_team (Team &team, std::string const &name, unsigned size);
void load_pokemon (Team &team, std::ifstream &file, species const pokemon_converter [], abilities const ability_converter [], items const item_converter [], natures const nature_converter [], moves_list const move_converter [], unsigned size);
unsigned converter (std::string const &data, std::string const &end, std::string const &line);

}

}
