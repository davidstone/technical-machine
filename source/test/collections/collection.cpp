// Test checked collections
// Copyright (C) 2015 David Stone
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

#include <bounded/integer_range.hpp>

#include <containers/array/make_array.hpp>
#include <containers/static_vector/static_vector.hpp>

#include <cassert>
#include <iostream>

namespace technicalmachine {
namespace {
using namespace bounded::literal;

struct TestValue {
	static constexpr auto class_name = "TestValue";
	constexpr TestValue(int v):
		m_value(v) {
	}
	constexpr operator int() const {
		return m_value;
	}
private:
	int m_value;
};

struct TestContainer {
	using value_type = TestValue;
	using Container = containers::static_vector<value_type, 7>;
	using const_iterator = typename Container::const_iterator;
	using size_type = Container::size_type;
	
	template<typename Container>
	constexpr TestContainer(Container const & container):
		m_container(begin(container), end(container)) {
	}
	friend constexpr auto begin(TestContainer const & container) {
		return begin(container.m_container);
	}
	friend constexpr auto end(TestContainer const & container) {
		return end(container.m_container);
	}
	
	CONTAINERS_OPERATOR_BRACKET_DEFINITIONS(TestContainer)
private:
	Container m_container;
};

void collection_range_tests() {
	constexpr auto const v = containers::make_array<TestValue>(2, 3, 5, 7, 11, 13, 17);
	using collection_type = detail::Collection<TestContainer>;
	collection_type base(v);
	assert(containers::size(v) == containers::size(base));
	base.set_index(containers::size(v) - 1_bi);
	if (base.index() != containers::size(v) - 1_bi) {
		throw InvalidCollection("Cannot set Collection index properly.");
	}
	if (base() != containers::back(v)) {
		throw InvalidCollection("Stored index in Collection does not return proper value.");
	}
	for (auto const n : bounded::integer_range(containers::size(v))) {
		if (v[n] != base(n)) {
			throw InvalidCollection("Specified index in Collection does not return proper value.");
		}
	}
}
}	// namespace

void collection_tests () {
	std::cout << "Running collection tests.\n";
	collection_range_tests();
	move_container_tests();
	move_collection_tests();
	variable_collection_tests();
	std::cout << "Collection tests passed.\n\n";
}

}	// namespace technicalmachine
