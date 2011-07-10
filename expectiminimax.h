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

namespace technicalmachine {

class score_variables;
class Team;
class Weather;

moves_list expectiminimax (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, int64_t &score);

int64_t select_move_branch (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool first_turn = false);

int64_t order_branch (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv);

int64_t accuracy_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

int64_t random_move_effects_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

int64_t awaken_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

int64_t use_move_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv);

int64_t end_of_turn_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv);

int64_t replace (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool faint, bool first_turn, bool verbose);

int64_t fainted (Team ai, Team foe, Weather weather, int depth, score_variables const &sv);

void deorder (Team &first, Team &last, Team* &ai, Team* &foe);

}
#endif
