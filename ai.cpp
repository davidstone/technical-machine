// AI to win a 1v1 battle
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
#include <boost/lexical_cast.hpp>
#include "ai.h"
#include "analyze_logs.h"
#include "expectiminimax.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "type.h"
#include "weather.h"

using namespace technicalmachine;

int main (int argc, char* argv[]) {
	int depth;
	if (argc == 1)
		depth = -1;
	else
		depth = boost::lexical_cast <int> (argv[1]);

	teams ai;
	teams foe;
	Weather weather;
	score_variables sv;
	Map map;
	int detailed [END_SPECIES][7] = {{ 0 }};
	initialize (ai, foe, sv, map, detailed);
	
	teams* first;
	teams* last;
	analyze_turn (ai, foe, first, last, weather, map);		// Turn 0, sending out initial Pokemon
	first->active->moved = false;
	first->active->move = first->active->moveset.begin();

	last->active->moved = false;
	last->active->move = last->active->moveset.begin();

	while ((ai.member.size() > 1 or ai.active->hp.stat > 0) and (foe.member.size() > 1 or foe.active->hp.stat > 0)) {
		teams predicted = foe;
		reset_iterators_pokemon (predicted);
		std::cout << "======================\nPredicting...\n";
		predict (detailed, predicted);
		std::cout << "======================\nEvaluating...\n";

		long score;
		moves_list best_move = expectiminimax (ai, predicted, weather, depth, sv, score);

		if (SWITCH1 <= best_move and best_move <= SWITCH6)
			std::cout << "Switch to " << pokemon_name [ai.member.at (best_move - SWITCH1).name];

		else
			std::cout << "Use " << move_name [best_move];
		if (depth == -1) {
			long double probability = 100.0 * static_cast <long double> (score + VICTORY) / static_cast <long double> (2 * VICTORY);
			std::cout << " for ";
			if ((8 <= probability and probability < 9) or (11 <= probability and probability < 12) or (18 <= probability and probability < 19) or (80 <= probability and probability < 90))
				std::cout << "an ";
			else
				std::cout << "a ";
			std::cout << probability << "% chance to win.\n";
		}
		else
			std::cout << " for a minimum expected score of " << score << "\n";

		analyze_turn (ai, foe, first, last, weather, map);
	}

	return 0;
}
