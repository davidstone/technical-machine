// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <filesystem>
#include <fstream>

namespace technicalmachine {

inline auto open_file(std::filesystem::path const & path, std::ios_base::openmode const mode = std::ios_base::in) -> std::ifstream {
	auto file = std::ifstream(path, mode);
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	return file;
}

} // namespace technicalmachine
