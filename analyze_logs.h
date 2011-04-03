// Analyze Shoddy Battle logs header
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
#include <map>
#include <string>
#include "pokemon.h"
#include "team.h"

namespace technicalmachine {

void log_pokemon  (teams &team, const std::string &line, const std::map <std::string, species> &species_map, std::string &search1);

void log_move (pokemon &member, pokemon* &previous, const std::string &line, const std::map <std::string, moves_list> &moves_map, const std::string &search);

void first (teams &team, std::ifstream &file, const std::map <std::string, species> &species_map);

void output (std::ofstream &output, const teams &team);

void isme (teams &team);

}
