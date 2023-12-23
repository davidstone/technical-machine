// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.ps.parsed_team;

import tm.ps_usage_stats.battle_result;
import tm.ps_usage_stats.battle_result_reader;
import tm.ps_usage_stats.battle_result_writer;
import tm.ps_usage_stats.rating;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.generation_generic;
import tm.get_directory;
import tm.item;
import tm.visible_hp;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto visible_hp(auto const hp) {
	return VisibleHP(CurrentVisibleHP(hp), MaxVisibleHP(hp));
}

auto make_result() {
	auto const team = ps::ParsedTeam({
		{
			.species = Species::Hippowdon,
			.gender = Gender::male,
			.item = Item::Leftovers,
			.ability = Ability::Sand_Stream,
			.stats = {
				visible_hp(420_bi),
				ps::ParsedStat(266_bi),
				ps::ParsedStat(352_bi),
				ps::ParsedStat(154_bi),
				ps::ParsedStat(190_bi),
				ps::ParsedStat(130_bi)
			},
			.moves = {{
				MoveName::Earthquake,
				MoveName::Slack_Off,
				MoveName::Stealth_Rock,
				MoveName::Roar
			}},
		},
		{
			.species = Species::Celebi,
			.item = Item::Leftovers,
			.ability = Ability::Natural_Cure,
			.stats = {
				visible_hp(404_bi),
				ps::ParsedStat(212_bi),
				ps::ParsedStat(319_bi),
				ps::ParsedStat(236_bi),
				ps::ParsedStat(238_bi),
				ps::ParsedStat(244_bi)
			},
			.moves = {{
				MoveName::Perish_Song,
				MoveName::Recover,
				MoveName::Grass_Knot,
				MoveName::U_turn
			}}
		},
		{
			.species = Species::Blissey,
			.gender = Gender::female,
			.item = Item::Leftovers,
			.ability = Ability::Natural_Cure,
			.stats = {
				visible_hp(651_bi),
				ps::ParsedStat(25_bi),
				ps::ParsedStat(119_bi),
				ps::ParsedStat(206_bi),
				ps::ParsedStat(385_bi),
				ps::ParsedStat(146_bi)
			},
			.moves = {{
				MoveName::Wish,
				MoveName::Soft_Boiled,
				MoveName::Toxic,
				MoveName::Flamethrower
			}}
		},
		{
			.species = Species::Skarmory,
			.gender = Gender::male,
			.item = Item::Shed_Shell,
			.ability = Ability::Keen_Eye,
			.stats = {
				visible_hp(334_bi),
				ps::ParsedStat(196_bi),
				ps::ParsedStat(402_bi),
				ps::ParsedStat(104_bi),
				ps::ParsedStat(185_bi),
				ps::ParsedStat(181_bi)
			},
			.moves = {{
				MoveName::Spikes,
				MoveName::Roost,
				MoveName::Brave_Bird,
				MoveName::Whirlwind
			}}
		},
		{
			.species = Species::Tentacruel,
			.gender = Gender::male,
			.item = Item::Leftovers,
			.ability = Ability::Liquid_Ooze,
			.stats = {
				visible_hp(364_bi),
				ps::ParsedStat(158_bi),
				ps::ParsedStat(196_bi),
				ps::ParsedStat(196_bi),
				ps::ParsedStat(341_bi),
				ps::ParsedStat(236_bi)
			},
			.moves = {{
				MoveName::Toxic_Spikes,
				MoveName::Rapid_Spin,
				MoveName::Surf,
				MoveName::Sludge_Bomb
			}}
		},
		{
			.species = Species::Rotom_Wash,
			.item = Item::Leftovers,
			.ability = Ability::Levitate,
			.stats = {
				visible_hp(304_bi),
				ps::ParsedStat(124_bi),
				ps::ParsedStat(284_bi),
				ps::ParsedStat(246_bi),
				ps::ParsedStat(308_bi),
				ps::ParsedStat(208_bi)
			},
			.moves = {{
				MoveName::Thunderbolt,
				MoveName::Shadow_Ball,
				MoveName::Reflect,
				MoveName::Hydro_Pump
			}}
		}
	});
	return ps_usage_stats::BattleResult{
		{team, 0_bi, tv::none},
		{team, 1_bi, ps_usage_stats::Rating(1.0, 1.0)},
		ps_usage_stats::BattleResult::Winner::tie
	};
}

TEST_CASE("Minimal team file", "[Minimal]") {
	auto const path = get_test_directory() / "teams" / "test.tmmt";
	auto const original = make_result();
	{
		auto writer = ps_usage_stats::BattleResultWriter(path);
		writer(original);
	}
	{
		auto reader = ps_usage_stats::battle_result_reader(path);
		CHECK(containers::size(reader) == 1_bi);
		CHECK(*reader.begin() == original);
	}
	std::filesystem::remove(path);
}

} // namespace
} // namespace technicalmachine
