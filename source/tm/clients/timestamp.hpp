// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <string>

namespace technicalmachine {

// Races with calls to std::gmtime, std::localtime, and std::ctime. Does not
// race with itself.
std::string timestamp();

}	// namespace technicalmachine
