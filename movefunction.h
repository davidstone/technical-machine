// Forward declaration of move functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef MOVEFUNCTION_H_
#define MOVEFUNCTION_H_

#include <map>
#include <string>
#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void move_priority (moves &move);
int usemove (Team &user, Team &target, Weather &weather, bool hitself, int old_damage = 0);
int usemove2 (Team &user, Team &target, Weather &weather, int old_damage);
void switchpokemon (Team &team, Pokemon &target, Weather &weather);
void blockexecution (Team &user, const Team &target, const Weather &weather, bool hitself);
void blockselection (Team &user, const Team &target, const Weather &weather);
bool block1 (const Team &user, const Team &target);
bool block2 (const Team &user, const Weather &weather);
bool imprison (const Team &user, const Team &target);
void set_move_map (std::map <std::string, moves_list> &moves_map);
void reset_variables (Pokemon &member);

}
#endif
