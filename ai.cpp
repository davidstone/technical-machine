// AI to win a 1v1 battle
// Copyright 2010 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.


#include <cstdlib>
#include <iostream>
#include "team.cpp"
#include "ai.h"
#include "item.h"
#include "move.h"
#include "pokemon.h"
#include "status.h"
#include "team.h"
#include "type.h"
#include "weather.h"
#include "test.h"
#include "stat.cpp"
#include "move.cpp"
#include "endofturn.cpp"
#include "expectiminimax.cpp"

int main (int argc, char* argv[]) {
	teams ai;
	teams foe;
	weathers weather;
	initialize (ai, foe, weather);
	ai.active->move = ai.active->moveset.begin();
	foe.active->move = foe.active->moveset.begin();
	ai.me = true;
	foe.me = false;
	int depth;
	if (argc == 1)
		depth = 1;
	else
		depth = atoi (argv[1]);
	int score;
	moves_list best_move = expectiminimax (ai, foe, weather, depth, score);
	std::cout << "Use " << move_name [best_move];
	if (-INT_MAX < score and score < INT_MAX)
		std::cout << " for a minimum expected score of " << score << "\n";
	else if (score == INT_MAX)
		std::cout << ", winning the game.\n";
	else
		std::cout << ", losing the game anyway.\n";
	return 0;
}
