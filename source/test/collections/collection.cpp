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

#include "invalid_collection.hpp"
#include "move_collection.hpp"
#include "move_container.hpp"
#include "variable_collection.hpp"
#include "../../collection.hpp"

#include <bounded_integer/integer_range.hpp>

#include <cassert>
#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

class TestContainer {
private:
	static constexpr intmax_t maximum = 7;
public:
	using size_type = bounded::checked_integer<0, maximum>;
	using index_type = bounded::checked_integer<0, maximum - 1>;
	using value_type = int;
	template<typename ... Args>
	TestContainer(Args && ... args):
		m_container(std::forward<Args>(args)...) {
	}
	size_type size() const {
		return static_cast<size_type>(m_container.size());
	}
	value_type const & operator[](index_type const index) const {
		return m_container[static_cast<std::size_t>(index)];
	}
private:
	std::vector<value_type> m_container;
};

void collection_range_tests() {
	constexpr auto size = 7_bi;
	std::vector<int> const v ({ 2, 3, 5, 7, 11, 13, 17 });
	using collection_type = detail::Collection<TestContainer>;
	collection_type base(v);
	assert(v.size() == size);
	base.set_index(size - 1_bi);
	if (base.index() != size - 1_bi)
		throw InvalidCollection("Cannot set Collection index properly.");
	if (base() != v.back())
		throw InvalidCollection("Stored index in Collection does not return proper value.");
	for (auto const n : bounded::integer_range(size)) {
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
