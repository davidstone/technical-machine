// Load Pokemon Lab teams
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_LAB_FILE_H_
#define POKEMON_LAB_FILE_H_

#include <fstream>
#include <string>

namespace technicalmachine {

class Map;
class Team;

namespace pl {

unsigned team_size (std::string const &name);
void load_team (Team &team, std::string const &name, Map const &map, unsigned size);
void load_pokemon (Team& team, std::ifstream &file, Map const &map, unsigned size);
std::string search (std::ifstream &file, std::string &output2, std::string const &data);

}

}
#endif
