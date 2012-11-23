// Move power calculator forward declarations
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

#ifndef MOVE__POWER_HPP_
#define MOVE__POWER_HPP_

namespace technicalmachine {

class ActivePokemon;
class Variable;
class Weather;

unsigned move_power(ActivePokemon const & attacker, ActivePokemon const & defender, Weather const & weather, Variable const & variable);

}	// namespace technicalmachine
#endif	// MOVE__POWER_HPP_
