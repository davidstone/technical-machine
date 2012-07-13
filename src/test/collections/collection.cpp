// Test checked collections
// Copyright (C) 2012 David Stone
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

#include "collection.hpp"
#include <iostream>
#include "invalid_collection.hpp"
#include "move_collection.hpp"
#include "move_container.hpp"
#include "variable_collection.hpp"
#include "../../collection.hpp"

namespace technicalmachine {
namespace {
void collection_range_tests() {
	std::vector<int> const v ({ 2, 3, 5, 7, 11, 13, 17 });
	detail::BaseCollection<int> base (v);
	base.set_index(v.size() - 1);
	if (base.index() != v.size() - 1)
		throw InvalidCollection("Cannot set Collection index properly.");
	if (base() != v.back())
		throw InvalidCollection("Stored index in Collection does not return proper value.");
	for (unsigned n = 0; n != v.size(); ++n) {
		if (v[n] != base(n))
			throw InvalidCollection("Specified index in Collection does not return proper value.");
	}
	try {
		base.set_index(v.size());
		throw InvalidCollection("Out of range index not caught.");
	}
	catch (InvalidCollectionIndex const &) {
		// Do nothing; the above operation should throw.
	}
	if (base() != v.back())
		throw InvalidCollection("Collection modified by setting an invalid index.");
}
}	// unnamed namespace

void collection_tests () {
	std::cout << "Running collection tests.\n";
	collection_range_tests();
	move_collection_tests();
	move_container_tests();
	variable_collection_tests();
	std::cout << "Collection tests passed.\n\n";
}

}	// namespace technicalmachine
