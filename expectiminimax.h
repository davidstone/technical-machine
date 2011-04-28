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

moves_list expectiminimax (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, long &score);
long tree1 (Team &ai, Team &foe, const Weather &weather, int depth, const score_variables &sv, moves_list &best_move, std::map<long, State> &transposition_table, bool first = false);
long tree2 (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, std::map<long, State> &transposition_table);
long tree3 (Team &ai, Team &foe, const Weather &weather, const int &depth, const score_variables &sv, Team* first, Team* last, std::map<long, State> &transposition_table);
long tree4 (const Team &ai, const Team &foe, const Weather &weather, const int &depth, const score_variables &sv, Team* first, Team* last, std::map<long, State> &transposition_table);
long tree5 (Team first, Team last, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table);
long tree6 (Team first, Team last, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table);
long fainted (Team ai, Team foe, Weather weather, int depth, const score_variables &sv, std::map<long, State> &transposition_table);

}
#endif
