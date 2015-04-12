// AI to win a 1v1 battle
// Copyright (C) 2014 David Stone
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

#include "clients/network/invalid_packet.hpp"
#include "clients/pokemon_online/client.hpp"
#include "clients/pokemon_showdown/client.hpp"

#include "pokemon/max_pokemon_per_team.hpp"

#include <boost/lexical_cast.hpp>

#include <iostream>

using namespace technicalmachine;

namespace {
void print_debug_statements();
}	// namespace

int main (int argc, char * argv []) {
	try {
		print_debug_statements();
		unsigned const depth = (argc == 1) ? 2 : boost::lexical_cast <unsigned> (argv [1]);

		// TODO: This is not the correct solution, but it works "good enough".
		// I'll get back to this later.
		bool stopping = false;
		while (!stopping) {
			try {
				po::Client client(depth);
				client.run();
				stopping = true;
			}
			catch (network::InvalidPacket const & error) {
				constexpr unsigned time_in_seconds = 10;
				std::cerr << error.what () << " Disconnected. Waiting " << time_in_seconds << " seconds and trying again.\n";
				// I disconnect from the server at this point and try again, because
				// this means an unrecoverable error.
				sleep (time_in_seconds);
				std::cerr << "Reconnecting.\n";
			}
		}
	}
	catch (...) {
		// This calls destructors to ensure orderly shut down but ruins core
		// dumps
		throw;
	}
}

namespace {
using namespace bounded::literal;

void print_debug_statements() {
	std::cout << "sizeof (Team): " << sizeof(Team) << '\n';
	std::cout << "sizeof (Pokemon): " << sizeof(Pokemon) << '\n';
	std::cout << "sizeof (Move): " << sizeof(Move) << '\n';
	constexpr auto heap_allocated_size = max_pokemon_per_team * (bounded::make<sizeof(Pokemon)>() + 4_bi * bounded::make<sizeof(Move)>());
	constexpr auto full_team_size = bounded::make<sizeof(Team)>() + heap_allocated_size;
	std::cout << "size of full team: " << full_team_size << '\n';
	std::cout << "heap allocated_size: " << heap_allocated_size << '\n';
}
}	// namespace
