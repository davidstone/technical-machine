// Copyright (C) 2017 David Stone
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

#pragma once

#include <bounded/integer.hpp>

namespace technicalmachine {

enum class Moves : std::uint16_t;
struct PP;
struct Team;
struct Variable;
struct Weather;

// If a damaging move does not have power (for instance, OHKO moves and
// fixed-damage moves), the behavior of this function is undefined.
using MovePower = bounded::integer<1, 1440>;
auto move_power(Team const & attacker, Moves move, PP pp, Team const & defender, Weather weather, Variable variable) -> MovePower;

}	// namespace technicalmachine
