// Stat function forward declarations
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATFUNCTION_H_
#define STATFUNCTION_H_

#include <map>
#include <string>
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

int hitpoints (Pokemon &member);
void attack (Team &attacker, Weather const &weather);
void defense (Team const &attacker, Team &defender, Weather const &weather);
void speed (Team &team, const Weather &weather);
void order (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower);
void faster_pokemon (Team &team1, Team &team2, Weather const &weather, Team* &faster, Team* &slower);
void set_nature_map (std::map <std::string, natures> &natures_map);

}
#endif
