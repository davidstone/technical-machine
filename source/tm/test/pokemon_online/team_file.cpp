// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.po.read_team_file;

import tm.move.move_name;

import tm.pokemon.happiness;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.test.pokemon_init;

import tm.ability;
import tm.bytes_in_file;
import tm.gender;
import tm.generation;
import tm.get_directory;
import tm.item;

import bounded;
import tv;
import std_module;

namespace technicalmachine {
namespace {

using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto expected_team = make_known_team<Generation::four>({
	{
		.species = Species::Bulbasaur,
		.gender = Gender::male,
		.item = Item::None,
		.ability = Ability::Overgrow,
		.ivs = IVs(IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
		.moves = {{
			MoveName::Light_Screen,
		}},
		.happiness = Happiness(0_bi),
	},
	{
		.species = Species::Wormadam_Sandy,
        .nickname = "Wormadam-G"sv,
		.gender = Gender::female,
		.item = Item::Bright_Powder,
		.ability = Ability::Anticipation,
        .nature = Nature::Bold,
		.evs = EVs(EV(0_bi), EV(0_bi), EV(0_bi), EV(0_bi), EV(88_bi), EV(0_bi)),
		.moves = {{
			MoveName::Dig,
			MoveName::Double_Team,
			MoveName::Sucker_Punch,
		}},
		.happiness = Happiness(0_bi),
	},
});

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
		<Move>389</Move>
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
	CHECK(original_team == expected_team);
}

} // namespace
} // namespace technicalmachine