// Status function forward declarations
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef STATUSFUNCTION_H_
#define STATUSFUNCTION_H_

#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void burn (Team &user, Team &target, const Weather &weather);
void freeze (const Pokemon &user, Team &target, const Weather &weather);
void paralyze (Pokemon &user, Pokemon &target, const Weather &weather);
void sleep (const Pokemon &user, Pokemon &target, const Weather &weather);
void poison_normal (Team &user, Team &target, const Weather &weather);
void poison_toxic (Team &user, Team &target, const Weather &weather);

}
#endif
