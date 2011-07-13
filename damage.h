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

namespace technicalmachine {

class Pokemon;
class Team;
class Weather;

void movepower (Team &attacker, Team const &defender, Weather const weather);

int damageknown (Team const &attacker, Team const &defender, Weather const &weather, int &rl, int &weather_mod, int &ff, int &mf);

int damagenonrandom (Team const &attacker, Team const &defender, int rl, int weather_mod, int ff, int mf, int &stab, int type1, int type2, int &aem, int &eb, int &tl, int &rb, int damage);

int damagerandom (Pokemon const &attacker, Team const &defender, int stab, int type1, int type2, int aem, int eb, int tl, int rb, int damage);

int damagecalculator (Team const &attacker, Team const &defender, Weather const &weather);

void recoil (Pokemon &user, int damage, int denominator);
}

#endif
