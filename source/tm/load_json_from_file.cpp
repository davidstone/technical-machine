// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <vector>

export module tm.load_json_from_file;

export import tm.nlohmann_json;
import tm.open_file;

import std_module;

namespace technicalmachine {

export auto load_json_from_file(std::filesystem::path const & path) -> nlohmann::json {
	auto json = nlohmann::json();
	auto file = open_text_file_for_reading(path);
	file >> json;
	return json;
}

} // namespace technicalmachine
