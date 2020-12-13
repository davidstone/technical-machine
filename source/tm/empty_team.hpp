// Exception if operations are performed on an empty team
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <filesystem>
#include <stdexcept>

namespace technicalmachine {

struct EmptyTeam : std::logic_error {
	EmptyTeam(std::filesystem::path const & file, unsigned const line):
		std::logic_error("Attempted operation on a team of size 0 at: " + file.string() + ": " + std::to_string(line) + ".") {
	}
};
}	// namespace technicalmachine
