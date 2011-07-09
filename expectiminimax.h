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

#include "move.h"

namespace technicalmachine {

class score_variables;
class Team;
class Weather;

moves_list expectiminimax (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, long &score);

long select_move_branch (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool first_turn = false);

long order_branch (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv);

long accuracy_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

long random_move_effects_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

long awaken_branch (Team &first, Team &last, Weather const &weather, int const &depth, score_variables const &sv);

long use_move_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv);

long end_of_turn_branch (Team first, Team last, Weather weather, int depth, score_variables const &sv);

long replace (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, bool faint, bool first_turn, bool verbose);

long fainted (Team ai, Team foe, Weather weather, int depth, score_variables const &sv);

void deorder (Team &first, Team &last, Team* &ai, Team* &foe);

}
#endif
