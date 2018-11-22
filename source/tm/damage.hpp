// Damage calculator forward declarations
// Copyright (C) 2016 David Stone
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

struct Move;
struct Pokemon;
struct Team;
struct Variable;
struct Weather;

using damage_type = bounded::checked_integer<0, std::numeric_limits<std::uint32_t>::max()>;

damage_type damage_calculator(Team const & attacker, Move move, bool attacker_damaged, Team const & defender, bool defender_damaged, Weather weather, Variable variable, bool critical_hit);

void recoil(Pokemon & user, damage_type damage, bounded::checked_integer<1, 4> denominator);

}	// namespace technicalmachine
