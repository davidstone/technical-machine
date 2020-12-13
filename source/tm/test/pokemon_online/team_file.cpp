// Test PO team files
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/pokemon_online/team_file.hpp>

#include <tm/clients/pokemon_online/read_team_file.hpp>
#include <tm/clients/pokemon_online/write_team_file.hpp>

#include <filesystem>
#include <iostream>

namespace technicalmachine {
namespace po {

void test_team_file () {
	constexpr auto generation = Generation::four;
	std::cout << "\tRunning Pokemon Online team file tests.\n";
	auto const directory = std::filesystem::path("test/teams");
	auto const new_file = directory / "test2.tp";
	auto const team = load_team<generation>(directory / "test1.tp");
	write_team(team, new_file);
	auto const new_team = load_team<generation>(new_file);
	BOUNDED_ASSERT(team == new_team);
	std::filesystem::remove(new_file);
}

}	// namespace po
}	// namespace technicalmachine
