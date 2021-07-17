// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <containers/vector.hpp>

#include <cstddef>
#include <fstream>

namespace technicalmachine {

auto bytes_in_file(std::ifstream file) -> containers::vector<std::byte>;

} // namespace technicalmachine
