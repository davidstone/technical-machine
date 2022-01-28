// Maximum amount of moves a Pokemon can have
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <bounded/integer.hpp>

#include <containers/static_vector.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;
using namespace bounded::literal;

constexpr auto max_moves_per_pokemon = 4_bi;
using RegularMoveSize = bounded::integer<1, static_cast<int>(max_moves_per_pokemon)>;

constexpr auto number_of_weird_moves = 2_bi; // Pass, Struggle
using SharedMoveSize = decltype(std::declval<TeamSize>() + number_of_weird_moves);

using MoveSize = decltype(std::declval<RegularMoveSize>() + std::declval<SharedMoveSize>());

using StaticVectorMove = containers::static_vector<
	Moves,
	numeric_traits::max_value<MoveSize>
>;

}	// namespace technicalmachine
