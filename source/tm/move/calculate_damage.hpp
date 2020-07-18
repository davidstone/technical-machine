// Copyright (C) 2019 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <tm/generation.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

struct ExecutedMove;
struct OtherMove;
template<Generation>
struct Team;
struct Weather;

using damage_type = bounded::checked_integer<0, 1'000'000'000>;

template<Generation generation>
auto calculate_damage(Team<generation> const & attacker, ExecutedMove, bool move_weakened_from_item, Team<generation> const & defender, OtherMove defender_move, Weather) -> damage_type;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto calculate_damage<generation>(Team<generation> const & attacker, ExecutedMove, bool move_weakened_from_item, Team<generation> const & defender, OtherMove defender_move, Weather) -> damage_type

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
