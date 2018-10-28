// Use moves
// Copyright (C) 2018 David Stone
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

#include <tm/move/move.hpp>
#include <tm/damage.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Move;
struct Team;
struct Variable;
struct Weather;

struct UsedMove {
	Move move;
	damage_type damage;
};

auto call_move(Team & user, Move move, bool user_damaged, Team & target, bounded::optional<UsedMove> target_move, bool target_damaged, Weather & weather, Variable const & variable, bool missed, bool awakens, bool critical_hit, bounded::optional<damage_type> known_damage) -> void;

}	// namespace technicalmachine
