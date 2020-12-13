// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/move/actual_damage.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

template<Generation>
struct Team;

struct Weather;

template<Generation generation>
auto call_move(Team<generation> & user, UsedMove<generation> move, Team<generation> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto call_move<generation>(Team<generation> & user, UsedMove<generation> move, Team<generation> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void

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
