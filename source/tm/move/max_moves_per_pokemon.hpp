// Maximum amount of moves a Pokemon can have
// Copyright (C) 2015 David Stone
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

#include <bounded/integer.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;
using namespace bounded::literal;

constexpr auto max_moves_per_pokemon = 4_bi;
using RegularMoveSize = bounded::integer<1, static_cast<int>(max_moves_per_pokemon)>;
using RegularMoveIndex = bounded::checked_integer<
	0,
	static_cast<int>(std::numeric_limits<RegularMoveSize>::max() - 1_bi)
>;

constexpr auto number_of_weird_moves = 2_bi; // Pass, Struggle
using SharedMoveSize = decltype(std::declval<TeamSize>() + number_of_weird_moves);
using SharedMoveIndex = bounded::checked_integer<
	0,
	static_cast<int>(std::numeric_limits<SharedMoveSize>::max() - 1_bi)
>;

using MoveSize = decltype(std::declval<RegularMoveSize>() + std::declval<SharedMoveSize>());

}	// namespace technicalmachine
