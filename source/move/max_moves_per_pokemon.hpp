// Maximum amount of moves a Pokemon can have
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

#ifndef MOVE__MAX_MOVES_PER_POKEMON_HPP_
#define MOVE__MAX_MOVES_PER_POKEMON_HPP_

#include "../pokemon/max_pokemon_per_team.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded_integer::literal;

using RegularMoveSize = bounded_integer::native_integer<1, 4>;
using RegularMoveIndex = bounded_integer::checked_integer<
	0,
	static_cast<intmax_t>(std::numeric_limits<RegularMoveSize>::max() - 1_bi)
>;

constexpr auto number_of_weird_moves = 1_bi;	// Struggle
using SharedMoveSize = decltype(std::declval<TeamSize>() + number_of_weird_moves);
using SharedMoveIndex = bounded_integer::checked_integer<
	0,
	static_cast<intmax_t>(std::numeric_limits<SharedMoveSize>::max() - 1_bi)
>;

using MoveSize = decltype(std::declval<RegularMoveSize>() + std::declval<SharedMoveSize>());
using MoveIndex = decltype(
	std::declval<RegularMoveIndex>() +
	std::declval<SharedMoveIndex>() +
	std::declval<bounded_integer::native_integer<0, 1>>()
);

}	// namespace technicalmachine
#endif	// MOVE__COLLECTION_HPP_
