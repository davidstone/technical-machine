// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.files_in_path;

import containers;
import std_module;

namespace technicalmachine {

export auto files_in_path(std::filesystem::path const & path) -> containers::vector<std::filesystem::path> {
	auto not_directory = [](std::filesystem::path const & p) { return !std::filesystem::is_directory(p); };
	if (not_directory(path)) {
		return containers::vector<std::filesystem::path>({path});
	}
	auto view = containers::subrange(
		std::filesystem::recursive_directory_iterator(path),
		std::default_sentinel
	);
	return containers::vector<std::filesystem::path>(containers::filter(view, not_directory));
}

} // namespace technicalmachine
