// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/damage_type.hpp>
#include <tm/move/other_move.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<any_team UserTeam>
struct ExecutedMove;

template<any_team UserTeam, any_team OtherTeamType>
auto calculate_damage(UserTeam const & attacker, ExecutedMove<UserTeam>, bool move_weakened_from_item, OtherTeamType const & defender, OtherMove defender_move, Weather) -> damage_type;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(UserTeam, OtherTeamType) \
	extern template auto calculate_damage(UserTeam const & attacker, ExecutedMove<UserTeam>, bool move_weakened_from_item, OtherTeamType const & defender, OtherMove defender_move, Weather) -> damage_type

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(Team<generation>, Team<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>, SeenTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>, SeenTeam<generation>)

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION
#undef TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL

} // namespace technicalmachine
