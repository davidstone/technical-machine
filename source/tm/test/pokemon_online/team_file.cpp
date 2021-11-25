// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_online/read_team_file.hpp>
#include <tm/clients/pokemon_online/write_team_file.hpp>

#include <tm/get_directory.hpp>

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

TEST_CASE("Pokemon Online team file", "[Pokemon Online]") {
	auto const directory = get_test_directory() / "teams";
	auto const new_file = directory / "test2.tp";
	auto const original_team = po::read_team_file(directory / "test1.tp");
	bounded::visit(original_team, [&](auto const & team) {
		po::write_team(team, new_file);
	});
	auto const new_team = po::read_team_file(new_file);
	CHECK(original_team == new_team);
	std::filesystem::remove(new_file);
}

} // namespace
} // namespace technicalmachine