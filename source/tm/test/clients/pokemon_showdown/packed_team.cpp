// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <catch2/catch_test_macros.hpp>

export module tm.test.clients.ps.packed_team;

import tm.clients.ps.packed_format_to_team;
import tm.clients.ps.to_packed_format;

import tm.generation;
import tm.generation_generic;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

TEST_CASE("Pokemon Showdown Generation 1 packed_team", "[pokemon showdown]") {
	constexpr auto initial = "Pika|Pikachu||Honey Gather|Thunderbolt,Surf|Hardy|252,4,248,252,252,252|Genderless|30,2,30,30,30,30||83|]Snorlax|||Honey Gather|Body Slam,Earthquake,Hyper Beam,Rest|Hardy|252,252,252,252,252,252|Genderless|30,30,30,30,30,30||100|"sv;
	constexpr auto generation = Generation::one;
	constexpr auto team = ps::packed_format_to_team<generation>(initial);
	CHECK(ps::to_packed_format(GenerationGeneric<KnownTeam>(team)) == initial);
}

TEST_CASE("Pokemon Showdown Generation 7 packed_team", "[pokemon showdown]") {
	constexpr auto initial = "Lumi|Lumineon|Focus Sash|Honey Gather|Defog,Scald,Ice Beam,U-turn|Serious|84,84,84,84,80,84|Genderless|31,31,31,31,31,31||83|]Glaceon||Life Orb|Honey Gather|Toxic,Hidden Power,Shadow Ball,Ice Beam|Serious|80,0,84,80,80,84|Genderless|31,3,31,31,31,31||83|]Crabominable||Choice Band|Honey Gather|Close Combat,Earthquake,Stone Edge,Ice Hammer|Serious|84,84,80,84,84,84|Genderless|31,31,31,31,31,31||83|]Toxicroak||Life Orb|Honey Gather|Drain Punch,Sucker Punch,Gunk Shot,Substitute|Serious|84,84,84,80,84,84|Genderless|31,31,31,31,31,31||79|]Bouffalant||Choice Band|Honey Gather|Earthquake,Megahorn,Head Charge,Superpower|Serious|80,84,80,84,80,84|Genderless|31,31,31,31,31,31||83|]Qwilfish||Black Sludge|Honey Gather|Thunder Wave,Destiny Bond,Liquidation,Pain Split|Serious|84,80,84,84,84,84|Genderless|31,31,31,31,31,31||83|"sv;
	constexpr auto generation = Generation::seven;
	constexpr auto team = ps::packed_format_to_team<generation>(initial);
	CHECK(ps::to_packed_format(GenerationGeneric<KnownTeam>(team)) == initial);
}

} // namespace technicalmachine
