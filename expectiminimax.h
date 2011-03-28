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

struct random_team {
	int length;			// Length of sleep
	bool shed_skin;		// Does Shed Skin activate?
};

struct Random {
	random_team first;
	random_team last;
};

moves_list expectiminimax (teams &ai, teams &foe, const weathers &weather, int depth, const score_variables &sv, long &score);
long tree1 (teams &ai, teams &foe, const weathers &weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long tree2 (teams &ai, teams &foe, const weathers &weather, const int &depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long tree3 (const teams &ai, const teams &foe, const weathers &weather, const int &depth, const score_variables &sv, moves_list &best_move, teams* first, teams* last, std::string &output, std::map<long, State> &transposition_table);
long tree4 (teams first, teams last, weathers weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long tree5 (teams first, teams last, weathers weather, const Random &random, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long tree6 (teams &first, teams &last, const weathers &weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
long tree7 (teams first, teams last, weathers weather, int depth, const score_variables &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table);
void reset_iterators (teams &team);
void reset_iterators_pokemon (teams &team);
void reset_iterators_move (pokemon &member);

#endif
