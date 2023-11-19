// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

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
import tm.team;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

auto make_result() {
	constexpr auto generation = Generation::four;
	auto const team = GenerationGeneric<Team>(Team<generation>({
		{
			Species::Hippowdon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Sand_Stream,
			CombinedStatsFor<generation>{
				Nature::Impish,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(24_bi), EV(192_bi), EV(0_bi), EV(40_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Earthquake),
				Move(generation, MoveName::Slack_Off),
				Move(generation, MoveName::Stealth_Rock),
				Move(generation, MoveName::Roar)
			})
		},
		{
			Species::Celebi,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Natural_Cure,
			CombinedStatsFor<generation>{
				Nature::Bold,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(216_bi), EV(0_bi), EV(8_bi), EV(32_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Perish_Song),
				Move(generation, MoveName::Recover),
				Move(generation, MoveName::Grass_Knot),
				Move(generation, MoveName::U_turn)
			})
		},
		{
			Species::Blissey,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Natural_Cure,
			CombinedStatsFor<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(0_bi), EV(0_bi), EV(252_bi), EV(80_bi), EV(176_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Wish),
				Move(generation, MoveName::Soft_Boiled),
				Move(generation, MoveName::Toxic),
				Move(generation, MoveName::Flamethrower)
			})
		},
		{
			Species::Skarmory,
			Level(100_bi),
			Gender::male,
			Item::Shed_Shell,
			Ability::Keen_Eye,
			CombinedStatsFor<generation>{
				Nature::Impish,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(200_bi), EV(0_bi), EV(36_bi), EV(20_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Spikes),
				Move(generation, MoveName::Roost),
				Move(generation, MoveName::Brave_Bird),
				Move(generation, MoveName::Whirlwind)
			})
		},
		{
			Species::Tentacruel,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Liquid_Ooze,
			CombinedStatsFor<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(120_bi), EV(0_bi), EV(136_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Toxic_Spikes),
				Move(generation, MoveName::Rapid_Spin),
				Move(generation, MoveName::Surf),
				Move(generation, MoveName::Sludge_Bomb)
			})
		},
		{
			Species::Rotom_Wash,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Levitate,
			CombinedStatsFor<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(136_bi), EV(0_bi), EV(120_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Thunderbolt),
				Move(generation, MoveName::Shadow_Ball),
				Move(generation, MoveName::Reflect),
				Move(generation, MoveName::Hydro_Pump)
			})
		}
	}));
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
