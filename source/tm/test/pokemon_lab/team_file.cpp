// Test PL team files
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

#include <tm/test/pokemon_lab/team_file.hpp>

#include <tm/clients/pokemon_lab/read_team_file.hpp>
#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <bounded/assert.hpp>

#include <filesystem>
#include <iostream>

namespace technicalmachine {
namespace pl {

void test_team_file () {
	constexpr auto generation = Generation::four;
	std::cout << "\tRunning Pokemon Lab team file tests.\n";
	auto const directory = std::filesystem::path("test/teams");
	auto const new_file = directory / "test2.sbt";
	auto const team = load_team(generation, directory / "test1.sbt");
	write_team(generation, team, new_file);
	BOUNDED_ASSERT(team == load_team(generation, new_file));
	std::filesystem::remove(new_file);
}

}	// namespace pl
}	// namespace technicalmachine
