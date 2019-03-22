// Convert between a switch and the equivalent Pokemon index
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

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;

auto is_switch(Moves name) -> bool;
auto to_switch(TeamIndex replacement) -> Moves;
auto to_replacement(Moves move) -> TeamIndex;

}	// namespace technicalmachine
