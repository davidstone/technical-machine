// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/damage_type.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<typename TeamType>
struct ExecutedMove;
struct OtherMove;
struct Weather;

template<any_team UserTeam>
auto calculate_damage(UserTeam const & attacker, ExecutedMove<UserTeam>, bool move_weakened_from_item, UserTeam const & defender, OtherMove defender_move, Weather) -> damage_type;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto calculate_damage(Team<generation> const & attacker, ExecutedMove<Team<generation>>, bool move_weakened_from_item, Team<generation> const & defender, OtherMove defender_move, Weather) -> damage_type

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
