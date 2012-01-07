// Test stuff
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

#include "string_conversion.hpp"
#include "pokemon_lab/test.hpp"
#include "pokemon_online/test.hpp"

int main () {
	using namespace technicalmachine;
	string_conversion_tests ();
	std::cout << "String conversion tests passed.\n\n";

	pl::test ();
	std::cout << "Pokemon Lab tests passed.\n\n";

	po::test ();
	std::cout << "Pokemon Online tests passed.\n\n";

	std::cout << "All tests passed.\n";
	return 0;
}
