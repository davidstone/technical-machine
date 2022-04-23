// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/open_file.hpp>

#include <nlohmann/json.hpp>

#include <filesystem>
#include <fstream>

namespace technicalmachine {

inline auto load_json_from_file(std::filesystem::path const & path) -> nlohmann::json {
	auto json = nlohmann::json();
	auto file = open_file(path, std::ios_base::in);
	file >> json;
	return json;
}

} // namespace technicalmachine
