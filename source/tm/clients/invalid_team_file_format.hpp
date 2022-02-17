// Exception if an invalid team file extension is given
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <filesystem>
#include <stdexcept>

namespace technicalmachine {

struct InvalidTeamFileFormat : std::logic_error {
	explicit InvalidTeamFileFormat(std::filesystem::path const & file);
};
} // namespace technicalmachine
