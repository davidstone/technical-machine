// Test PL team files
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

#include "team_file.hpp"

#include <iostream>

#include "../../team.hpp"
#include "../../clients/pokemon_lab/read_team_file.hpp"
#include "../../clients/pokemon_lab/write_team_file.hpp"

namespace technicalmachine {
namespace pl {

void test_team_file () {
	std::cout << "\tRunning Pokemon Lab team file tests.\n";
	Team team(max_pokemon_per_team);
	std::string const directory = "source/test/";
	load_team (team, directory + "test1.sbt");
	write_team (team, directory + "test2.sbt");
}

}	// namespace pl
}	// namespace technicalmachine
