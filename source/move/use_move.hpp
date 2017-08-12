// Use moves
// Copyright (C) 2016 David Stone
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

#include <bounded/optional.hpp>

namespace technicalmachine {

enum class Moves : std::uint16_t;

struct Move;
struct Team;
struct Variable;
struct Weather;

auto call_move(Team & user, Move move, Team & target, bounded::optional<Moves> target_move, Weather & weather, Variable const & variable, bool missed, bool awakens, bool critical_hit, bool damage_is_known) -> void;

}	// namespace technicalmachine
