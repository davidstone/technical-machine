// Convert between a switch and the equivalent Pokemon index
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/max_pokemon_per_team.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;

auto is_switch(Moves name) -> bool;
auto to_switch(TeamIndex replacement) -> Moves;
auto to_replacement(Moves move) -> TeamIndex;

}	// namespace technicalmachine
