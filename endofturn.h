// End of turn function forward declaration
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef ENDOFTURN_H_
#define ENDOFTURN_H_

#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void endofturn (Team &first, Team &last, Weather &weather);
void endofturn0 (Team &team);
void endofturn1 (Team &team);
void endofturn2 (Team &team);
void endofturn3 (Team &team, const Weather &weather);
void endofturn5 (Team &team, Pokemon &foe, Weather &weather);
void endofturn6 (Team &target, const Weather &weather);
void endofturn7 (Team &team);
void decrement (char &n);

}

#endif
