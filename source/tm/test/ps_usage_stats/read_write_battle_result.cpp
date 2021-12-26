// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/battle_result.hpp>
#include <tm/ps_usage_stats/battle_result_reader.hpp>
#include <tm/ps_usage_stats/battle_result_writer.hpp>

#include <tm/get_directory.hpp>

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto make_result() {
	constexpr auto generation = Generation::four;
	auto const team = GenerationGeneric<Team>(Team<generation>({
		{
			Species::Hippowdon,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Sand_Stream,
			CombinedStats<generation>{
				Nature::Impish,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(24_bi), EV(192_bi), EV(0_bi), EV(40_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, Moves::Earthquake),
				Move(generation, Moves::Slack_Off),
				Move(generation, Moves::Stealth_Rock),
				Move(generation, Moves::Roar)
			})
		},
		{
			Species::Celebi,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Natural_Cure,
			CombinedStats<generation>{
				Nature::Bold,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(216_bi), EV(0_bi), EV(8_bi), EV(32_bi))
			},
			RegularMoves({
				Move(generation, Moves::Perish_Song),
				Move(generation, Moves::Recover),
				Move(generation, Moves::Grass_Knot),
				Move(generation, Moves::U_turn)
			})
		},
		{
			Species::Blissey,
			Level(100_bi),
			Gender::female,
			Item::Leftovers,
			Ability::Natural_Cure,
			CombinedStats<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(0_bi), EV(0_bi), EV(252_bi), EV(80_bi), EV(176_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, Moves::Wish),
				Move(generation, Moves::Soft_Boiled),
				Move(generation, Moves::Toxic),
				Move(generation, Moves::Flamethrower)
			})
		},
		{
			Species::Skarmory,
			Level(100_bi),
			Gender::male,
			Item::Shed_Shell,
			Ability::Keen_Eye,
			CombinedStats<generation>{
				Nature::Impish,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(200_bi), EV(0_bi), EV(36_bi), EV(20_bi))
			},
			RegularMoves({
				Move(generation, Moves::Spikes),
				Move(generation, Moves::Roost),
				Move(generation, Moves::Brave_Bird),
				Move(generation, Moves::Whirlwind)
			})
		},
		{
			Species::Tentacruel,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Liquid_Ooze,
			CombinedStats<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(120_bi), EV(0_bi), EV(136_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, Moves::Toxic_Spikes),
				Move(generation, Moves::Rapid_Spin),
				Move(generation, Moves::Surf),
				Move(generation, Moves::Sludge_Bomb)
			})
		},
		{
			Species::Rotom_Wash,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Calm,
				IVs(IV(31_bi), IV(3_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(252_bi), EV(0_bi), EV(136_bi), EV(0_bi), EV(120_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, Moves::Thunderbolt),
				Move(generation, Moves::Shadow_Ball),
				Move(generation, Moves::Reflect),
				Move(generation, Moves::Hydro_Pump)
			})
		}
	}));
	return ps_usage_stats::BattleResult{
		{team, 0_bi, bounded::none},
		{team, 1_bi, ps_usage_stats::Rating{1.0, 1.0}},
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
