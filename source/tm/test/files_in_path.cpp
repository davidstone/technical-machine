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

#include <tm/test/files_in_path.hpp>

#include <tm/files_in_path.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/sort.hpp>
#include <containers/scope_guard.hpp>

#include <fstream>
#include <iostream>

namespace technicalmachine {

auto test_files_in_path() -> void {
	auto const path = std::filesystem::temp_directory_path() / "tm_test_files_in_path";

	auto remove_temp_directory = [&]{ std::filesystem::remove_all(path); };
	remove_temp_directory();
	std::filesystem::create_directory(path);
	auto const guard = containers::scope_guard(remove_temp_directory);

	std::filesystem::create_directory(path / "c");
	std::ofstream(path / "a");
	std::ofstream(path / "b");
	std::ofstream(path / "c" / "d");

	auto files = files_in_path(path);
	containers::sort(files);
	BOUNDED_ASSERT(files == containers::vector{path / "a", path / "b", path / "c/d"});
}

} // namespace technicalmachine
