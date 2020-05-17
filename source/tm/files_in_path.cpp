// Copyright (C) 2020 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
