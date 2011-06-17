// Determine when transpositions occur
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <map>
#include <string>
#include "evaluate.h"
#include "expectiminimax.h"
#include "move.h"
#include "pokemon.h"
#include "state.h"
#include "team.h"
#include "transposition.h"
#include "weather.h"

namespace technicalmachine {
	
// This currently generates no hits. I'm currently working on this to find out why.

long transposition (Team &ai, Team &foe, Weather const &weather, int const &depth, score_variables const &sv, moves_list &best_move, std::string &output, std::map<long, State> &transposition_table) {
	reset_iterators (ai);	
	reset_iterators (foe);
	State state;		// This causes possibly needless copying. Changing my code elsewhere to use the State data structure could solve this problem.

	if (depth == 0)
		state.score = evaluate (ai, foe, weather, sv);
	else {
		state.ai = ai;
		state.foe = foe;
		state.weather = weather;
		state.depth = depth;
		long const hash = hash_state (state, sv);
		std::map<long, State>::iterator it = transposition_table.find (hash);
		// If I can find the current state in my transposition table at a depth of at least the current depth, set the score to the stored score.
		if (it != transposition_table.end() and it->second.depth >= depth) {
//			std::cout << "Need for speed\n";
			state.score = it->second.score;
		}
		else {
			// If I can't find it, set the score to the evaluation of the state at depth - 1.
			state.score = tree1 (state.ai, state.foe, state.weather, state.depth, sv, best_move, output, transposition_table);
			
			// If I didn't find any stored value at the same hash as the current state, add it to my table. If the value I found was for a search at a shallower depth, replace it with the new value.
			if (it == transposition_table.end() or state.depth >= it->second.depth) {
				transposition_table [hash] = state;
			}
		}
	}
	return state.score;
}

}
