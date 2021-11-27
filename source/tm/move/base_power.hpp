// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/executed_move.hpp>

#include <tm/any_team.hpp>
#include <tm/generation.hpp>
#include <tm/weather.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

// Fling gives 0, Rollout gives 480
using BasePower = bounded::integer<0, 480>;

// It is undefined behavior to get the base power of a move without a base power
// (Dragon Range, Guillotine, etc.).
template<any_team UserTeam>
auto base_power(UserTeam const & attacker_team, ExecutedMove<UserTeam>, UserTeam const & defender_team, Weather) -> BasePower;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto base_power(Team<generation> const &, ExecutedMove<Team<generation>>, Team<generation> const &, Weather) -> BasePower

TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::one);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::two);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::three);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::four);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::five);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::six);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::seven);
TECHNICALMACHINE_EXTERN_INSTANTIATION(Generation::eight);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION

} // namespace technicalmachine
