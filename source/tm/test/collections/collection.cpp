// Test checked collections
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/collections/collection.hpp>

#include <tm/test/collections/move_container.hpp>
#include <tm/test/collections/variable_collection.hpp>

#include <iostream>

namespace technicalmachine {

void collection_tests () {
	std::cout << "Running collection tests.\n";
	move_container_tests();
	variable_collection_tests();
	std::cout << "Collection tests passed.\n\n";
}

}	// namespace technicalmachine
