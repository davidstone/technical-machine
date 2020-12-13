// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/known_move.hpp>
#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

using PossibleExecutedMoves = containers::static_vector<KnownMove, 3>;

template<Generation generation>
auto possible_executed_moves(Moves const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto possible_executed_moves<generation>(Moves const selected_move, Team<generation> const & user_team) -> PossibleExecutedMoves

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

}	// namespace technicalmachine
