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

#include "expectiminimax.h"
#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void endofturn (teams &first, teams &last, Weather &weather, const Random &random);
void endofturn0 (pokemon &member);
void endofturn1 (teams &team);
void endofturn2 (teams &team);
void endofturn3 (pokemon &member, const Weather &weather);
void endofturn5 (pokemon &member, pokemon &foe, Weather &weather, const random_team &random);
void endofturn6 (teams &target, const Weather &weather);
void endofturn7 (pokemon &member);

}

#endif
