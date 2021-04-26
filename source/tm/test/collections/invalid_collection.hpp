// Test checked collections
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <stdexcept>
#include <string_view>

namespace technicalmachine {

struct InvalidCollection : std::logic_error {
	explicit InvalidCollection(std::string_view str);
};

}	// namespace technicalmachine

