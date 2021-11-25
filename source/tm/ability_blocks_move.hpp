// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/type/pokemon_types.hpp>
#include <tm/ability.hpp>
#include <tm/generation.hpp>
#include <tm/status.hpp>

namespace technicalmachine {

auto ability_blocks_move(Generation const generation, Ability const ability, KnownMove const move, Statuses const target_status, PokemonTypes const target_types) -> bool;

} // namespace technicalmachine
