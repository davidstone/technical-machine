// Damage calculator forward declarations
// Copyright (C) 2012 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef DAMAGE_HPP_
#define DAMAGE_HPP_

namespace technicalmachine {

class ActivePokemon;
class Pokemon;
class Team;
class Weather;

unsigned damage_calculator (ActivePokemon const & attacker, Team const & defender, Weather const & weather);
unsigned uncapped_damage (ActivePokemon const & attacker, Team const & defender, Weather const & weather);
void recoil (Pokemon & user, unsigned damage, unsigned denominator);

}	// namespace technicalmachine
#endif	// DAMAGE_HPP_
