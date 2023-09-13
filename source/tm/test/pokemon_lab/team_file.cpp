// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.pl.read_team_file;
import tm.clients.pl.write_team_file;

import tm.bytes_in_file;
import tm.get_directory;

import tv;
import std_module;

namespace technicalmachine {
namespace {

using namespace std::string_view_literals;

constexpr auto team_xml = R"(
<?xml version="1.0" encoding="UTF-8" ?>
<shoddybattle generation="4">

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

TEST_CASE("Pokemon Lab team file", "[Pokemon Lab]") {
	auto const original_team = pl::read_team_file(std::as_bytes(std::span(team_xml)));
	auto const directory = get_test_directory() / "teams";
	auto const new_file = directory / "test2.sbt";
	tv::visit(original_team, [&](auto const & team) {
		pl::write_team(team, new_file);
	});
	auto const new_team = pl::read_team_file(bytes_in_file(new_file));
	CHECK(original_team == new_team);
	std::filesystem::remove(new_file);
}

} // namespace
} // namespace technicalmachine
