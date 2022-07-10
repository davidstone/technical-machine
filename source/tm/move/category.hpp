// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/generation.hpp>

#include <cstdint>

namespace technicalmachine {

auto is_physical(Generation, KnownMove) -> bool;
auto is_special(Generation, KnownMove) -> bool;
auto is_damaging(MoveName) -> bool;

} // namespace technicalmachine
