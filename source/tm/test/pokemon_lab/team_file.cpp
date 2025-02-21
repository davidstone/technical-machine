// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <catch2/catch_test_macros.hpp>

import tm.clients.pl.read_team_file;

import tm.move.move_name;

import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.ability;
import tm.gender;
import tm.initial_team;
import tm.item;

import bounded;
import std_module;

namespace technicalmachine {
namespace {

using namespace bounded;
using namespace std::string_view_literals;

constexpr auto team_xml = R"(
<?xml version="1.0" encoding="UTF-8" ?>
<shoddybattle>

<pokemon species="Hippowdon">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>Male</gender>
	<nature>Impish</nature>
	<item>Leftovers</item>
	<ability>Sand Stream</ability>
	<moveset>
		<move pp-up="3">Earthquake</move>
		<move pp-up="3">Slack Off</move>
		<move pp-up="3">Stealth Rock</move>
		<move pp-up="3">Roar</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="252" />
		<stat name="Atk" iv="31" ev="24" />
		<stat name="Def" iv="31" ev="192" />
		<stat name="Spd" iv="31" ev="0" />
		<stat name="SpAtk" iv="31" ev="0" />
		<stat name="SpDef" iv="31" ev="40" />
	</stats>
</pokemon>

<pokemon species="Celebi">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>No Gender</gender>
	<nature>Bold</nature>
	<item>Leftovers</item>
	<ability>Natural Cure</ability>
	<moveset>
		<move pp-up="3">Perish Song</move>
		<move pp-up="3">Recover</move>
		<move pp-up="3">Grass Knot</move>
		<move pp-up="3">U-turn</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="252" />
		<stat name="Atk" iv="31" ev="0" />
		<stat name="Def" iv="31" ev="216" />
		<stat name="Spd" iv="31" ev="32" />
		<stat name="SpAtk" iv="31" ev="0" />
		<stat name="SpDef" iv="31" ev="8" />
	</stats>
</pokemon>

<pokemon species="Blissey">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>Female</gender>
	<nature>Calm</nature>
	<item>Leftovers</item>
	<ability>Natural Cure</ability>
	<moveset>
		<move pp-up="3">Wish</move>
		<move pp-up="3">Softboiled</move>
		<move pp-up="3">Toxic</move>
		<move pp-up="3">Flamethrower</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="0" />
		<stat name="Atk" iv="3" ev="0" />
		<stat name="Def" iv="31" ev="252" />
		<stat name="Spd" iv="31" ev="0" />
		<stat name="SpAtk" iv="31" ev="80" />
		<stat name="SpDef" iv="31" ev="176" />
	</stats>
</pokemon>

<pokemon species="Skarmory">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>Male</gender>
	<nature>Impish</nature>
	<item>Shed Shell</item>
	<ability>Keen Eye</ability>
	<moveset>
		<move pp-up="3">Spikes</move>
		<move pp-up="3">Roost</move>
		<move pp-up="3">Brave Bird</move>
		<move pp-up="3">Whirlwind</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="252" />
		<stat name="Atk" iv="31" ev="0" />
		<stat name="Def" iv="31" ev="200" />
		<stat name="Spd" iv="31" ev="20" />
		<stat name="SpAtk" iv="31" ev="0" />
		<stat name="SpDef" iv="31" ev="36" />
	</stats>
</pokemon>

<pokemon species="Tentacruel">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>Male</gender>
	<nature>Calm</nature>
	<item>Leftovers</item>
	<ability>Liquid Ooze</ability>
	<moveset>
		<move pp-up="3">Toxic Spikes</move>
		<move pp-up="3">Rapid Spin</move>
		<move pp-up="3">Surf</move>
		<move pp-up="3">Sludge Bomb</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="252" />
		<stat name="Atk" iv="31" ev="0" />
		<stat name="Def" iv="31" ev="120" />
		<stat name="Spd" iv="31" ev="0" />
		<stat name="SpAtk" iv="31" ev="0" />
		<stat name="SpDef" iv="31" ev="136" />
	</stats>
</pokemon>

<pokemon species="Rotom-w">
	<nickname></nickname>
	<level>100</level>
	<happiness>255</happiness>
	<gender>No Gender</gender>
	<nature>Calm</nature>
	<item>Leftovers</item>
	<ability>Levitate</ability>
	<moveset>
		<move pp-up="3">Thunderbolt</move>
		<move pp-up="3">Shadow Ball</move>
		<move pp-up="3">Reflect</move>
		<move pp-up="3">Hydro Pump</move>
	</moveset>
	<stats>
		<stat name="HP" iv="31" ev="252" />
		<stat name="Atk" iv="3" ev="0" />
		<stat name="Def" iv="31" ev="136" />
		<stat name="Spd" iv="31" ev="0" />
		<stat name="SpAtk" iv="31" ev="0" />
		<stat name="SpDef" iv="31" ev="120" />
	</stats>
</pokemon>

</shoddybattle>
)"sv;

constexpr auto expected = InitialTeam<SpecialInputStyle::split>({
	{
		.species = Species::Hippowdon,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.ability = Ability::Sand_Stream,
		.stats = {
			.nature = Nature::Impish,
			.evs = {EV(252_bi), EV(24_bi), EV(192_bi), EV(0_bi), EV(40_bi), EV(0_bi)}
		},
		.moves = {{
			MoveName::Earthquake,
			MoveName::Slack_Off,
			MoveName::Stealth_Rock,
			MoveName::Roar,
		}}
	},
	{
		.species = Species::Celebi,
		.item = Item::Leftovers,
		.ability = Ability::Natural_Cure,
		.stats = {
			.nature = Nature::Bold,
			.evs = {EV(252_bi), EV(0_bi), EV(216_bi), EV(0_bi), EV(8_bi), EV(32_bi)}
		},
		.moves = {{
			MoveName::Perish_Song,
			MoveName::Recover,
			MoveName::Grass_Knot,
			MoveName::U_turn,
		}}
	},
	{
		.species = Species::Blissey,
		.gender = Gender::female,
		.item = Item::Leftovers,
		.ability = Ability::Natural_Cure,
		.stats = {
			.nature = Nature::Calm,
			.dvs_or_ivs = {IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)},
			.evs = {EV(0_bi), EV(0_bi), EV(252_bi), EV(80_bi), EV(176_bi), EV(0_bi)}
		},
		.moves = {{
			MoveName::Wish,
			MoveName::Soft_Boiled,
			MoveName::Toxic,
			MoveName::Flamethrower,
		}}
	},
	{
		.species = Species::Skarmory,
		.gender = Gender::male,
		.item = Item::Shed_Shell,
		.ability = Ability::Keen_Eye,
		.stats = {
			.nature = Nature::Impish,
			.evs = {EV(252_bi), EV(0_bi), EV(200_bi), EV(0_bi), EV(36_bi), EV(20_bi)}
		},
		.moves = {{
			MoveName::Spikes,
			MoveName::Roost,
			MoveName::Brave_Bird,
			MoveName::Whirlwind,
		}}
	},
	{
		.species = Species::Tentacruel,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.ability = Ability::Liquid_Ooze,
		.stats = {
			.nature = Nature::Calm,
			.evs = {EV(252_bi), EV(0_bi), EV(120_bi), EV(0_bi), EV(136_bi), EV(0_bi)}
		},
		.moves = {{
			MoveName::Toxic_Spikes,
			MoveName::Rapid_Spin,
			MoveName::Surf,
			MoveName::Sludge_Bomb,
		}}
	},
	{
		.species = Species::Rotom_Wash,
		.nickname = "Rotom-w"sv,
		.item = Item::Leftovers,
		.ability = Ability::Levitate,
		.stats = {
			.nature = Nature::Calm,
			.dvs_or_ivs = {IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)},
			.evs = {EV(252_bi), EV(0_bi), EV(136_bi), EV(0_bi), EV(120_bi), EV(0_bi)}
		},
		.moves = {{
			MoveName::Thunderbolt,
			MoveName::Shadow_Ball,
			MoveName::Reflect,
			MoveName::Hydro_Pump,
		}}
	},
});

TEST_CASE("Pokemon Lab team file", "[Pokemon Lab]") {
	auto const parsed = pl::read_team_file(std::as_bytes(std::span(team_xml)));
	CHECK(parsed == expected);
}

} // namespace
} // namespace technicalmachine
