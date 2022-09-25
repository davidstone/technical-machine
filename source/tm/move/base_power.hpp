// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/executed_move.hpp>

#include <tm/any_team.hpp>
#include <tm/for_each_generation.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

// Fling gives 0, Rollout gives 480
using BasePower = bounded::integer<0, 480>;

// It is undefined behavior to get the base power of a move without a base power
// (Dragon Rage, Guillotine, etc.).
template<any_team UserTeam, any_team DefenderTeam>
auto base_power(UserTeam const & attacker_team, ExecutedMove<UserTeam>, DefenderTeam const & defender_team, Weather) -> BasePower;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(UserTeam, DefenderTeam) \
	extern template auto base_power(UserTeam const &, ExecutedMove<UserTeam>, DefenderTeam const &, Weather) -> BasePower

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(Team<generation>, Team<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>, SeenTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>, KnownTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>, SeenTeam<generation>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION
#undef TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL

} // namespace technicalmachine
