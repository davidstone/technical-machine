// Miscellaneous functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef SIMPLE_H_
#define SIMPLE_H_

#include "pokemon.h"
#include "type.h"
#include "weather.h"

namespace technicalmachine {

bool istype (const pokemon &member, types type);
void heal (pokemon &member, int denominator, int numerator = 1);
void decrement (char &n);
bool grounded (const pokemon &member, const weathers &weather);
void recoil (pokemon &user, int damage, int denominator);

}
#endif
