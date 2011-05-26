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

#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

int usemove (Team &user, Team &target, Weather &weather, int log_damage = -1);
int usemove2 (Team &user, Team &target, Weather &weather, int damage);
void do_damage (Team &user, Team &target, int damage);
void lower_pp (Team &user, const Pokemon &target);

}
#endif
