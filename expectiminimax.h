// Expectiminimax header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef EXPECTIMINIMAX_H_
#define EXPECTIMINIMAX_H_

#include <cstdint>
#include "move.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

class Score;

Move::Moves expectiminimax (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, int64_t & min_score);

int64_t select_move_branch (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, Move::Moves & best_move, bool first_turn = false);

int64_t order_branch (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score);

int64_t accuracy_branch (Team & first, Team & last, Weather const & weather, int depth, Score const & score);

int64_t random_move_effects_branch (Team & first, Team & last, Weather const & weather, int depth, Score const & score);

int64_t awaken_branch (Team & first, Team & last, Weather const & weather, int depth, Score const & score);

int64_t use_move_branch (Team first, Team last, Weather weather, int depth, Score const & score);

int64_t end_of_turn_branch (Team first, Team last, Weather weather, int depth, Score const & score);

int64_t end_of_turn_order_branch (Team & team, Team & other, Team * first, Team * last, Weather const & weather, int depth, Score const & score);

int64_t replace (Team & ai, Team & foe, Weather const & weather, int depth, Score const & score, Move::Moves & best_move, bool first_turn, bool verbose);

int64_t fainted (Team ai, Team foe, Weather weather, int depth, Score const & score);

void deorder (Team & first, Team & last, Team* & ai, Team* & foe);

int64_t move_then_switch_branch (Team & switcher, Team const & other, Weather const & weather, int depth, Score const & score, Move::Moves & best_switch);

int64_t switch_after_move_branch (Team switcher, Team other, Weather weather, int depth, Score const & score);

void print_best_move (Team const & team, Move::Moves best_move, int depth, int64_t score);

void print_action (Team const & team, bool verbose, bool first_turn, std::string indent);

}
#endif
