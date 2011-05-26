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

#include "evaluate.h"
#include "move.h"
#include "state.h"
#include "team.h"
#include "weather.h"

namespace technicalmachine {

moves_list expectiminimax (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, long &score);
long tree1 (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, std::map<long, State> &transposition_table, bool first_turn = false);
long tree2 (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv, std::map<long, State> &transposition_table);
long tree3 (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv, Team* first, Team* last, std::map<long, State> &transposition_table);
long tree4 (Team const &ai, Team const &foe, Weather const &weather, int const &depth, score_variables const &sv, Team* first, Team* last, std::map<long, State> &transposition_table);
long tree5 (Team first, Team last, Weather weather, int depth, score_variables const &sv, std::map<long, State> &transposition_table);
long tree6 (Team first, Team last, Weather weather, int depth, score_variables const &sv, std::map<long, State> &transposition_table);
long replace (Team &ai, Team &foe, Weather const &weather, int depth, score_variables const &sv, moves_list &best_move, std::map<long, State> &transposition_table, bool faint, bool first_turn);
long fainted (Team ai, Team foe, Weather weather, int depth, score_variables const &sv, std::map<long, State> &transposition_table);
void deorder (Team &first, Team &last, Team* &ai, Team* &foe);

}
#endif
