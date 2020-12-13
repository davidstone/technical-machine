// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/files_in_path.hpp>

#include <containers/algorithms/filter_iterator.hpp>
#include <containers/range_view.hpp>
#include <containers/single_element_range.hpp>

namespace technicalmachine {

auto files_in_path(std::filesystem::path const & path) -> containers::vector<std::filesystem::path> {
	auto not_directory = [](std::filesystem::path const & p) { return !std::filesystem::is_directory(p); };
	if (not_directory(path)) {
		return containers::vector<std::filesystem::path>(containers::single_element_range(path));
	}
	auto view = containers::range_view(
		std::filesystem::recursive_directory_iterator(path),
		std::filesystem::recursive_directory_iterator()
	);
	return containers::vector<std::filesystem::path>(containers::filter(view, not_directory));
}

} // namespace technicalmachine
