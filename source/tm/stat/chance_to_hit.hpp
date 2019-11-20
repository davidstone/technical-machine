// Chance to hit calculations
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

#include <cstdint>

namespace technicalmachine {

enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;
struct ActivePokemon;
struct Weather;

using ChanceToHit = double;

auto chance_to_hit(Generation generation, ActivePokemon user, Moves move, ActivePokemon target, Weather weather, bool target_moved) -> ChanceToHit;

}	// namespace technicalmachine
