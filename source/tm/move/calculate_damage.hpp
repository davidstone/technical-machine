// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<typename TeamType>
struct ExecutedMove;
struct OtherMove;
template<Generation>
struct Team;
struct Weather;

using damage_type = bounded::integer<0, 1'000'000'000>;

template<Generation generation>
auto calculate_damage(Team<generation> const & attacker, ExecutedMove<Team<generation>>, bool move_weakened_from_item, Team<generation> const & defender, OtherMove defender_move, Weather) -> damage_type;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto calculate_damage<generation>(Team<generation> const & attacker, ExecutedMove<Team<generation>>, bool move_weakened_from_item, Team<generation> const & defender, OtherMove defender_move, Weather) -> damage_type

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
