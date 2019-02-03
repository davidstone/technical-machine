// Copyright (C) 2019 David Stone
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

#pragma once

#include <containers/append.hpp>
#include <containers/range_view.hpp>
#include <containers/vector.hpp>

#include <filesystem>

namespace technicalmachine {

inline auto paths_in_directory(std::filesystem::path const & directory) {
	return containers::range_view(
		std::filesystem::directory_iterator(directory),
		std::filesystem::directory_iterator()
	);
}

inline auto recursive_files_in_path(std::filesystem::path const & path) -> containers::vector<std::filesystem::path> {
	auto files = containers::vector<std::filesystem::path>{};
	if (std::filesystem::is_directory(path)) {
		for (auto const & file : paths_in_directory(path)) {
			containers::append(files, recursive_files_in_path(file.path()));
		}
	} else if (std::filesystem::is_regular_file(path)) {
		push_back(files, path);
	}
	return files;
}

}	// namespace technicalmachine
