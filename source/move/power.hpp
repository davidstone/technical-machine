// Move power calculator forward declarations
// Copyright (C) 2014 David Stone
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

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

class Team;
class Variable;
class Weather;

using MovePower = bounded::integer<1, 1440>;
auto move_power(Team const & attacker, Team const & defender, Weather const & weather, Variable const & variable) -> MovePower;

}	// namespace technicalmachine
#endif	// MOVE__POWER_HPP_
