// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/actual_damage.hpp>
#include <tm/any_team.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <tm/any_team.hpp>
#include <tm/for_each_generation.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Weather;

template<any_team UserTeam>
auto call_move(UserTeam & user, UsedMove<UserTeam> move, OtherTeam<UserTeam> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(UserTeam) \
	extern template auto call_move(UserTeam & user, UsedMove<UserTeam> move, OtherTeam<UserTeam> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(Team<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(SeenTeam<generation>); \
	TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL(KnownTeam<generation>)

TECHNICALMACHINE_FOR_EACH_GENERATION(TECHNICALMACHINE_EXTERN_INSTANTIATION);

#undef TECHNICALMACHINE_EXTERN_INSTANTIATION
#undef TECHNICALMACHINE_EXTERN_INSTANTIATION_IMPL

} // namespace technicalmachine
