// AI to win a 1v1 battle
// Copyright (C) 2018 David Stone
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

#include <tm/clients/pokemon_showdown/client.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <boost/lexical_cast.hpp>

#include <iostream>
#include <thread>

using namespace technicalmachine;

namespace {

void print_debug_statements() {
	std::cout << "sizeof(Team): " << sizeof(Team) << '\n';
	std::cout << "sizeof(Pokemon): " << sizeof(Pokemon) << '\n';
	std::cout << "sizeof(Move): " << sizeof(Move) << '\n';
}

}	// namespace

int main(int argc, char * * argv) {
	print_debug_statements();
	unsigned const depth = (argc == 1) ? 2 : boost::lexical_cast<unsigned>(argv[1]);

	while (true) {
		try {
			auto client = ps::Client(load_settings_file("settings/settings.xml"), depth);
			client.run();
		} catch (std::exception const & ex) {
			constexpr auto timeout = std::chrono::seconds(10);
			std::cerr << ex.what() << " Disconnected. Waiting " << timeout.count() << " seconds and trying again.\n";
			std::this_thread::sleep_for(timeout);
			std::cerr << "Reconnecting.\n";
		}
	}
}

