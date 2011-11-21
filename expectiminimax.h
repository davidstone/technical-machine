// Expectiminimax header
// Copyright (C) 2011 David Stone
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

#ifndef EXPECTIMINIMAX_H_
#define EXPECTIMINIMAX_H_

#include <cstdint>
#include "move.h"

namespace technicalmachine {

class Score;
class Team;
class Weather;

Move::Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, int64_t & min_score);

int64_t select_move_branch (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, Move::Moves & best_move, bool first_turn = false);

}	// namespace technicalmachine
#endif	// EXPECTIMINIMAX_H_
