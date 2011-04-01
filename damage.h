// Damage calculator forward declarations
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef DAMAGE_H_
#define DAMAGE_H_

#include "pokemon.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

void movepower (pokemon &attacker, const pokemon &defender, const weathers weather);

int damageknown (const pokemon &attacker, const teams &defender, const weathers &weather, int &rl, int &weather_mod, int &ff, int &mf);

int damagenonrandom (const pokemon &attacker, const teams &defender, const int &rl, const int &weather_mod, const int &ff, const int &mf, int &stab, const int &type1, const int &type2, int &aem, int &eb, int &tl, int &rb, int damage);

int damagerandom (const pokemon &attacker, const teams &defender, const int &stab, const int &type1, const int &type2, const int &aem, const int &eb, const int &tl, const int &rb, int damage);

int damagecalculator (const pokemon &attacker, const teams &defender, const weathers &weather);

}

#endif
