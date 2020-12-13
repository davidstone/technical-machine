// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <containers/vector.hpp>

#include <filesystem>

namespace technicalmachine {

auto files_in_path(std::filesystem::path const & path) -> containers::vector<std::filesystem::path>;

} // namespace technicalmachine
