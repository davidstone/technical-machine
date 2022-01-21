// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

namespace technicalmachine {

inline auto load_json_from_file(std::filesystem::path const & path) -> nlohmann::json {
	auto json = nlohmann::json();
	auto file = std::ifstream(path);
	file.exceptions(std::ios_base::badbit | std::ios_base::failbit);
	file >> json;
	return json;
}

} // namespace technicalmachine
