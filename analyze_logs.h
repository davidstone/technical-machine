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
#include "weather.h"

namespace technicalmachine {

bool analyze_turn (Team &ai, Team &foe, Team* &first, Team* &last, Weather &weather, const Map &map);

void log_pokemon  (Team &team, Team &target, Weather &weather, const std::string &line, const Map &map, const std::string &search1);

void log_move (Team &user, Team &target, Weather &weather, const std::string &line, const Map &map, const std::string &search);

void log_misc (Pokemon &active, Pokemon &inactive, const std::string &line, const Map &map);

void output (std::string &output, const Team &team);

}
