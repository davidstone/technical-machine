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

void movepower (Team & attacker, Team const & defender, Weather const weather);

unsigned damageknown (Team const & attacker, Team const & defender, Weather const & weather, unsigned & rl, unsigned & weather_mod, unsigned & ff, unsigned & mf);

unsigned damagenonrandom (Team const & attacker, Team const & defender, unsigned rl, unsigned weather_mod, unsigned ff, unsigned mf, unsigned & stab, unsigned type1, unsigned type2, unsigned & aem, unsigned & eb, unsigned & tl, unsigned & rb, unsigned damage);

unsigned damagerandom (Pokemon const & attacker, Team const & defender, unsigned stab, unsigned type1, unsigned type2, unsigned aem, unsigned eb, unsigned tl, unsigned rb, unsigned damage);

unsigned damagecalculator (Team const & attacker, Team const & defender, Weather const & weather);

void recoil (Pokemon & user, unsigned damage, unsigned denominator);

void damage_side_effect (Pokemon & user, unsigned damage);

}
#endif
