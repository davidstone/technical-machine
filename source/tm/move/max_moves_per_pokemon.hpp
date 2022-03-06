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

// The possible selection sets are:
// Pass
// Struggle, team size (6) - 1 switches
// Normal moves (4), team size (6) - 1 switches
inline constexpr auto maximum_possible_selections = max_moves_per_pokemon + max_pokemon_per_team - 1_bi;

using LegalSelections = containers::static_vector<
	Moves,
	maximum_possible_selections
>;

} // namespace technicalmachine
