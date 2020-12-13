// String conversions template
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string_view>

namespace technicalmachine {

template<typename T>
auto from_string(std::string_view str) -> T;

}	// namespace technicalmachine
