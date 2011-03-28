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
#include "weather.h"

void burn (pokemon &user, pokemon &target, const weathers &weather);
void freeze (const pokemon &user, pokemon &target, const weathers &weather);
void paralyze (pokemon &user, pokemon &target, const weathers &weather);
void sleep (const pokemon &user, pokemon &target, const weathers &weather);
void poison_normal (pokemon &user, pokemon &target, const weathers &weather);
void poison_toxic (pokemon &user, pokemon &target, const weathers &weather);

#endif
