// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/for_each_generation.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/static_vector.hpp>

namespace technicalmachine {

using PossibleExecutedMoves = containers::static_vector<KnownMove, 3_bi>;

auto possible_executed_moves(MoveName const selected_move, any_team auto const & user_team) -> PossibleExecutedMoves;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto possible_executed_moves(MoveName const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine
