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

#include <tm/move/actual_damage.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

template<Generation>
struct Team;

struct Weather;

template<Generation generation>
auto call_move(Team<generation> & user, UsedMove move, Team<generation> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void;

#define TECHNICALMACHINE_EXTERN_INSTANTIATION(generation) \
	extern template auto call_move<generation>(Team<generation> & user, UsedMove move, Team<generation> & other, OtherMove other_move, Weather & weather, bool clear_status, ActualDamage actual_damage) -> void

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
