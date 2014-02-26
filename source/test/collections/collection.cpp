// Test checked collections
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

#include "collection.hpp"
#include <cassert>
#include <iostream>
#include "invalid_collection.hpp"
#include "move_collection.hpp"
#include "move_container.hpp"
#include "variable_collection.hpp"
#include "../../collection.hpp"

namespace technicalmachine {
namespace {
using namespace bounded_integer::literal;

void collection_range_tests() {
	constexpr auto size = 7_bi;
	std::vector<int> const v ({ 2, 3, 5, 7, 11, 13, 17 });
	using collection_type = detail::Collection<std::vector<int>, size.value()>;
	collection_type base(v);
	assert(v.size() == size);
	base.set_index(size - 1_bi);
	if (base.index() != size - 1_bi)
		throw InvalidCollection("Cannot set Collection index properly.");
	if (base() != v.back())
		throw InvalidCollection("Stored index in Collection does not return proper value.");
	for (auto const n : bounded_integer::range(size)) {
		if (v[static_cast<std::size_t>(n)] != base(n)) {
			throw InvalidCollection("Specified index in Collection does not return proper value.");
		}
	}
}
}	// unnamed namespace

void collection_tests () {
	std::cout << "Running collection tests.\n";
	collection_range_tests();
	move_container_tests();
	move_collection_tests();
	variable_collection_tests();
	std::cout << "Collection tests passed.\n\n";
}

}	// namespace technicalmachine
