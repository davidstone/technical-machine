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
#include <tm/move/move.hpp>

#include <bounded/optional.hpp>

namespace technicalmachine {

struct Pokemon;
struct Team;
struct Variable;
struct Weather;

struct UsedMove {
	Move move;
	damage_type damage;
};

damage_type damage_calculator(Team const & attacker, Move move, Team const & defender, bounded::optional<UsedMove> defender_move, bool defender_damaged, Weather weather, Variable variable, bool critical_hit);

}	// namespace technicalmachine
