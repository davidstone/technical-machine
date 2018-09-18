// Expectiminimax header
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

#include <random>
#include "../move/moves.hpp"

namespace technicalmachine {

struct Evaluate;
struct Team;
struct Weather;

Moves expectiminimax(Team const & ai, Team const & foe, Weather weather, unsigned depth, Evaluate evaluate, std::mt19937 & random_engine);

// Called from the function that identifies transpositions
struct BestMove {
	Moves move;
	double score;
};
BestMove select_type_of_move(Team const & ai, Team const & foe, Weather weather, unsigned depth, Evaluate evaluate, bool first_turn = false);

}	// namespace technicalmachine
