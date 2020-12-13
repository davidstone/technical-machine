// Test PL team files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
	auto const team = load_team<generation>(directory / "test1.sbt");
	write_team(team, new_file);
	BOUNDED_ASSERT(team == load_team<generation>(new_file));
	std::filesystem::remove(new_file);
}

}	// namespace pl
}	// namespace technicalmachine
