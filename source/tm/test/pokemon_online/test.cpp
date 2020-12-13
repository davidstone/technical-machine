// Test PO stuff
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/pokemon_online/test.hpp>

#include <iostream>

#include <tm/test/pokemon_online/conversion.hpp>
#include <tm/test/pokemon_online/team_file.hpp>

namespace technicalmachine {
namespace po {

void test () {
	std::cout << "Running Pokemon Online tests.\n";
	test_conversions ();
	test_team_file ();
	std::cout << "Pokemon Online tests passed.\n\n";
}

}	// namespace po
}	// namespace technicalmachine
