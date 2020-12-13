// Test PL stuff
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/pokemon_lab/test.hpp>

#include <iostream>

#include <tm/test/pokemon_lab/team_file.hpp>

namespace technicalmachine {
namespace pl {

void test () {
	std::cout << "Running Pokemon Lab tests.\n";
	test_team_file();
	std::cout << "Pokemon Lab tests passed.\n\n";
}

}	// namespace pl
}	// namespace technicalmachine
