// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.pl.read_team_file;
import tm.clients.pl.write_team_file;

import tm.get_directory;

import tv;
import std_module;

namespace technicalmachine {
namespace {

TEST_CASE("Pokemon Lab team file", "[Pokemon Lab]") {
	auto const directory = get_test_directory() / "teams";
	auto const new_file = directory / "test2.sbt";
	auto const original_team = pl::read_team_file(directory / "test1.sbt");
	tv::visit(original_team, [&](auto const & team) {
		pl::write_team(team, new_file);
	});
	auto const new_team = pl::read_team_file(new_file);
	CHECK(original_team == new_team);
	std::filesystem::remove(new_file);
}

} // namespace
} // namespace technicalmachine
