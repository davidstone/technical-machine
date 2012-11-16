// Expectiminimax header
// Copyright (C) 2012 David Stone
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

#ifndef EVALUATE__EXPECTIMINIMAX_HPP_
#define EVALUATE__EXPECTIMINIMAX_HPP_

#include <cstdint>
#include <random>
#include "../move/moves_forward.hpp"

namespace technicalmachine {

class Evaluate;
class MoveScores;
class Team;
class Weather;

Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, unsigned depth, Evaluate const & evaluate, std::mt19937 & random_engine);

// Called from the function that identifies transpositions
int64_t select_type_of_move_branch (Team & ai, Team & foe, MoveScores & ai_scores, MoveScores & foe_scores, Weather const & weather, unsigned depth, Evaluate const & evaluate, Moves & best_move, bool first_turn = false);

}	// namespace technicalmachine
#endif	// EVALUATE__EXPECTIMINIMAX_HPP_
