// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.move.call_move;

import tm.move.actual_damage;
import tm.move.other_move;
import tm.move.used_move;

import tm.any_team;
import tm.environment;
import tm.other_team;
import tm.team;

namespace technicalmachine {

export template<any_team UserTeam>
auto call_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherMove const other_move, Environment & environment, bool const clear_status, ActualDamage const actual_damage, bool const is_fully_paralyzed) -> void;

#define EXTERN_INSTANTIATION_ONE(UserTeam) \
	extern template auto call_move(UserTeam & user, UsedMove<UserTeam> const move, OtherTeam<UserTeam> & other, OtherMove const other_move, Environment & environment, bool const clear_status, ActualDamage const actual_damage, bool const is_fully_paralyzed) -> void

#define EXTERN_INSTANTIATION_ALL(generation) \
	EXTERN_INSTANTIATION_ONE(Team<generation>); \
	EXTERN_INSTANTIATION_ONE(KnownTeam<generation>); \
	EXTERN_INSTANTIATION_ONE(SeenTeam<generation>)

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION_ALL);

} // namespace technicalmachine
