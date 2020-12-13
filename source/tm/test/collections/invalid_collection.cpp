// Test checked collections
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/collections/invalid_collection.hpp>

namespace technicalmachine {

InvalidCollection::InvalidCollection(std::string const & str) :
	std::logic_error(str) {
}

}	// namespace technicalmachine
