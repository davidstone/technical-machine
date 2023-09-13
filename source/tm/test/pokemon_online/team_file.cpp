// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.po.read_team_file;
import tm.clients.po.write_team_file;

import tm.bytes_in_file;
import tm.get_directory;

import tv;
import std_module;

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

constexpr auto team_xml = R"(
<Team version="1" gen="4" defaultTier="">
    <Trainer loseMsg="" avatar="1" winMsg="" infoMsg="">TM</Trainer>
    <Pokemon Item="0" Ability="65" Num="1" Nature="0" Shiny="0" Nickname="Bulbasaur" Gen="4" Forme="0" Happiness="0" Lvl="100" Gender="1">
        <Move>113</Move>
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>3</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
    </Pokemon>
    <Pokemon Item="3" Ability="107" Num="413" Nature="5" Shiny="0" Nickname="Wormadam-G" Gen="4" Forme="1" Happiness="0" Lvl="100" Gender="2">
        <Move>91</Move>
        <Move>104</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>88</EV>
        <EV>0</EV>
    </Pokemon>
    <Pokemon Item="0" Ability="0" Num="0" Nature="0" Shiny="0" Nickname="Missingno" Gen="4" Forme="0" Happiness="0" Lvl="100" Gender="0">
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
    </Pokemon>
    <Pokemon Item="0" Ability="0" Num="0" Nature="0" Shiny="0" Nickname="Missingno" Gen="4" Forme="0" Happiness="0" Lvl="100" Gender="0">
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
    </Pokemon>
    <Pokemon Item="0" Ability="0" Num="0" Nature="0" Shiny="0" Nickname="Missingno" Gen="4" Forme="0" Happiness="0" Lvl="100" Gender="0">
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
    </Pokemon>
    <Pokemon Item="0" Ability="0" Num="0" Nature="0" Shiny="0" Nickname="Missingno" Gen="4" Forme="0" Happiness="0" Lvl="100" Gender="0">
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <Move>0</Move>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <DV>31</DV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
        <EV>0</EV>
    </Pokemon>
</Team>
)"sv;

TEST_CASE("Pokemon Online team file", "[Pokemon Online]") {
	auto const original_team = po::read_team_file(std::as_bytes(std::span(team_xml)));
	auto const directory = get_test_directory() / "teams";
	auto const new_file = directory / "test2.tp";
	tv::visit(original_team, [&](auto const & team) {
		po::write_team(team, new_file);
	});
	auto const new_team = po::read_team_file(bytes_in_file(new_file));
	CHECK(original_team == new_team);
	std::filesystem::remove(new_file);
}

} // namespace
} // namespace technicalmachine