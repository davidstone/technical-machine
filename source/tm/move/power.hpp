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

#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
struct Team;

// If a damaging move does not have power (for instance, OHKO moves and
// fixed-damage moves), the behavior of this function is undefined. If
// `executed.move.name` is Hidden Power, `attacker.pokemon().hidden_power()`
// must not be `none`.
using MovePower = bounded::integer<1, 1440>;
auto move_power(Generation const generation, Team const & attacker, ExecutedMove executed, Team const & defender, Weather weather) -> MovePower;

}	// namespace technicalmachine
