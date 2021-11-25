// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_lab/read_team_file.hpp>
#include <tm/clients/pokemon_lab/write_team_file.hpp>

#include <tm/get_directory.hpp>

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

TEST_CASE("Pokemon Lab team file", "[Pokemon Lab]") {
	auto const directory = get_test_directory() / "teams";
	auto const new_file = directory / "test2.sbt";
	auto const original_team = pl::read_team_file(directory / "test1.sbt");
	bounded::visit(original_team, [&](auto const & team) {
		pl::write_team(team, new_file);
	});
	auto const new_team = pl::read_team_file(new_file);
	CHECK(original_team == new_team);
	std::filesystem::remove(new_file);
}

} // namespace
} // namespace technicalmachine
