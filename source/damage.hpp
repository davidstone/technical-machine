// Damage calculator forward declarations
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

#ifndef DAMAGE_HPP_
#define DAMAGE_HPP_

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

class Pokemon;
class Team;
class Variable;
class Weather;

using damage_type = bounded::equivalent_type<unsigned, bounded::throw_policy>;

damage_type damage_calculator(Team const & attacker, Team const & defender, Weather weather, Variable const & variable);

void recoil (Pokemon & user, damage_type damage, bounded::checked_integer<1, 4> denominator);

}	// namespace technicalmachine
#endif	// DAMAGE_HPP_
