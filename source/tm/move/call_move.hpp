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

#include <tm/move/damage_type.hpp>
#include <tm/move/executed_move.hpp>
#include <tm/move/other_move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Team;
struct Variable;
struct Weather;

auto call_move(Team & user, ExecutedMove move, Team & other, OtherMove other_move, Weather & weather, Variable variable, bool missed, bool clear_status, bool critical_hit, bounded::optional<damage_type> known_damage) -> void;

}	// namespace technicalmachine
