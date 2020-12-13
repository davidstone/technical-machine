// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/test/files_in_path.hpp>

#include <tm/files_in_path.hpp>

#include <bounded/assert.hpp>

#include <containers/algorithms/sort.hpp>
#include <bounded/scope_guard.hpp>

#include <fstream>
#include <iostream>

namespace technicalmachine {

auto test_files_in_path() -> void {
	auto const path = std::filesystem::temp_directory_path() / "tm_test_files_in_path";

	auto remove_temp_directory = [&]{ std::filesystem::remove_all(path); };
	remove_temp_directory();
	std::filesystem::create_directory(path);
	auto const guard = bounded::scope_guard(remove_temp_directory);

	std::filesystem::create_directory(path / "c");
	std::ofstream(path / "a");
	std::ofstream(path / "b");
	std::ofstream(path / "c" / "d");

	auto files = files_in_path(path);
	containers::sort(files);
	BOUNDED_ASSERT(files == containers::vector({path / "a", path / "b", path / "c/d"}));
}

} // namespace technicalmachine
