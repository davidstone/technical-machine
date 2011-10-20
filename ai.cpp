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


#include <cstdint>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "analyze_logs.h"
#include "expectiminimax.h"
#include "load_stats.h"
#include "team.h"
#include "teampredictor.h"
#include "weather.h"
#include "pokemon.h"
#include "pokemon_lab/connect.h"
#include "pokemon_online/connect.h"
#include "network/invalid_packet.h"

int main (int argc, char* argv[]) {
	using namespace technicalmachine;

	int depth;
	if (argc == 1)
		depth = 2;
	else
		depth = boost::lexical_cast <int> (argv[1]);

	// TODO: This is not the correct solution, but it works "good enough". I'll back back to this later.
	while (true) {
		try {
			po::Client client (depth);
			client.run();
		}
		catch (network::InvalidPacket & error) {
			std::cerr << error.what () << "\n";
			// I disconnect from the server at this point and try again, because this means an unrecoverable error.
		}
	}

/*	unsigned const foe_size = 6;
	int detailed [END_SPECIES][7] = {{ 0 }};
	detailed_stats (detailed);
	Team ai (true, foe_size);
	Team foe (false, ai.size);
	Weather weather;
	Score score;
	analyze_turn (ai, foe, weather);		// Turn 0, sending out initial Pokemon

	bool won = false;
	while (!won) {
		Team predicted = foe;
		std::cout << "======================\nPredicting...\n";
		predict_team (detailed, predicted, ai.size);
		std::string out;
		predicted.output (out);
		std::cout << out;

		int64_t min_score;
		expectiminimax (ai, predicted, weather, depth, score, min_score);

		won = analyze_turn (ai, foe, weather);
	}
*/
	return 0;
}
