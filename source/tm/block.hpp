// Declaration of functions that block selection / execution
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

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/move.hpp>

#include <containers/static_vector/static_vector.hpp>

namespace technicalmachine {

struct ActivePokemon;
struct MutableActivePokemon;
struct Team;
struct Weather;

using StaticVectorMove = containers::static_vector<
	Moves,
	bounded::detail::builtin_max_value<MoveSize>
>;

auto legal_selections(Team const & user, ActivePokemon other, Weather weather) -> StaticVectorMove;
auto can_attempt_move_execution(ActivePokemon user, Move move, ActivePokemon other) -> bool;
auto can_execute_move(ActivePokemon user, Move move, Weather weather, bool is_recharging) -> bool;

}	// namespace technicalmachine
