// Test checked collections
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/collections/invalid_collection.hpp>

#include <string>

namespace technicalmachine {

InvalidCollection::InvalidCollection(std::string_view const str) :
	std::logic_error(std::string(str))
{
}

}	// namespace technicalmachine
