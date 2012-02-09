// AI to win a 1v1 battle
// Copyright (C) 2011 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#include <iostream>

#include <boost/lexical_cast.hpp>

#include "pokemon_lab/connect.hpp"
#include "pokemon_online/connect.hpp"
#include "network/invalid_packet.hpp"

int main (int argc, char * argv []) {
	using namespace technicalmachine;

	int depth;
	if (argc == 1)
		depth = 2;
	else
		depth = boost::lexical_cast <int> (argv [1]);

	// TODO: This is not the correct solution, but it works "good enough". I'll get back to this later.
	while (true) {
		try {
			po::Client client (depth);
			client.run();
		}
		catch (network::InvalidPacket const & error) {
			constexpr unsigned time_in_seconds = 10;
			std::cerr << error.what () << " Disconnected. Waiting " << time_in_seconds << " seconds and trying again.\n";
			// I disconnect from the server at this point and try again, because this means an unrecoverable error.
			sleep (time_in_seconds);
			std::cerr << "Reconnecting.\n";
		}
	}
	return 0;
}
