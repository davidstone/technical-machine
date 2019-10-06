// Copyright (C) 2019 David Stone
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

#include <tm/move/executed_move.hpp>
#include <tm/move/other_move.hpp>

namespace technicalmachine {

enum class Generation;
struct Pokemon;
struct Team;
struct Weather;

using damage_type = bounded::checked_integer<0, 4294967295>;

auto calculate_uncapped_damage(Generation const generation, Team const & attacker, ExecutedMove const move, Team const & defender, OtherMove const defender_move, Weather const weather) -> damage_type;

auto calculate_damage(Generation generation, Team const & attacker, ExecutedMove move, Team const & defender, OtherMove defender_move, Weather weather) -> HP::current_type;

}	// namespace technicalmachine
