// Declaration of functions that block selection / execution
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef BLOCK_H_
#define BLOCK_H

#include "move.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void blockexecution (Team &user, const Team &target, const Weather &weather);
void blockselection (Team &user, const Team &target, const Weather &weather);
bool block1 (const Team &user, const Team &target);
bool block2 (const Team &user, const Weather &weather);
bool imprison (const Move &move, const Team &target);

}

#endif
