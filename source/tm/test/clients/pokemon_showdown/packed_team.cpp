// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_showdown/packed_format_to_team.hpp>
#include <tm/clients/pokemon_showdown/to_packed_format.hpp>

#include <containers/algorithms/concatenate.hpp>
#include <containers/begin_end.hpp>

#include <string_view>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

TEST_CASE("Pokemon Showdown packed_team", "[pokemon showdown]") {
	using namespace std::string_view_literals;
	// TODO: test exact example string from PS documentation at
	// https://github.com/Zarel/Pokemon-Showdown/blob/master/PROTOCOL.md#team-format
	constexpr auto initial = "Lumineon||Focus Sash|Honey Gather|Defog,Scald,Ice Beam,U-turn|Serious|84,84,84,84,80,84|Genderless|31,31,31,31,31,31||83|]Glaceon||Life Orb|Honey Gather|Toxic,Hidden Power,Shadow Ball,Ice Beam|Serious|80,0,84,80,80,84|Genderless|31,3,31,31,31,31||83|]Crabominable||Choice Band|Honey Gather|Close Combat,Earthquake,Stone Edge,Ice Hammer|Serious|84,84,80,84,84,84|Genderless|31,31,31,31,31,31||83|]Toxicroak||Life Orb|Honey Gather|Drain Punch,Sucker Punch,Gunk Shot,Substitute|Serious|84,84,84,80,84,84|Genderless|31,31,31,31,31,31||79|]Bouffalant||Choice Band|Honey Gather|Earthquake,Megahorn,Head Charge,Superpower|Serious|80,84,80,84,80,84|Genderless|31,31,31,31,31,31||83|]Qwilfish||Black Sludge|Honey Gather|Thunder Wave,Destiny Bond,Liquidation,Pain Split|Serious|84,80,84,84,84,84|Genderless|31,31,31,31,31,31||83|"sv;
	constexpr auto generation = Generation::seven;
	auto const team = ps::packed_format_to_team<generation>(initial);
	auto const output = ps::to_packed_format(team);
	CHECK(output == initial);
}

} // namespace
} // namespace technicalmachine
