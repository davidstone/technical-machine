// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/netbattle/read_team_file.hpp>

#include <tm/string_conversions/pokemon.hpp>
#include <tm/string_conversions/team.hpp>

#include <tm/get_directory.hpp>

#include <filesystem>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto expected_netbattle_team() -> Team<Generation::three> {
	constexpr auto generation = Generation::three;
	return Team<generation>({
		Pokemon<generation>(
			Species::Forretress,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Sturdy,
			CombinedStats<generation>{
				Nature::Jolly,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(88_bi), EV(84_bi), EV(84_bi), EV(0_bi), EV(0_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Earthquake),
				Move(generation, Moves::Explosion),
				Move(generation, Moves::Rapid_Spin),
				Move(generation, Moves::Spikes)
			})
		),
		Pokemon<generation>(
			Species::Houndoom,
			Level(100_bi),
			Gender::male,
			Item::Salac_Berry,
			Ability::Flash_Fire,
			CombinedStats<generation>{
				Nature::Naive,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(0_bi), EV(252_bi), EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Crunch),
				Move(generation, Moves::Endure),
				Move(generation, Moves::Fire_Blast),
				Move(generation, Moves::Reversal)
			})
		),
		Pokemon<generation>(
			Species::Gengar,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Levitate,
			CombinedStats<generation>{
				Nature::Timid,
				IVs(IV(31_bi), IV(0_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(0_bi), EV(0_bi), EV(84_bi), EV(0_bi), EV(172_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Dream_Eater),
				Move(generation, Moves::Hypnosis),
				Move(generation, Moves::Mean_Look),
				Move(generation, Moves::Nightmare)
			})
		),
		Pokemon<generation>(
			Species::Heracross,
			Level(100_bi),
			Gender::male,
			Item::Salac_Berry,
			Ability::Swarm,
			CombinedStats<generation>{
				Nature::Jolly,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(56_bi), EV(128_bi), EV(0_bi), EV(0_bi), EV(72_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Endure),
				Move(generation, Moves::Flail),
				Move(generation, Moves::Megahorn),
				Move(generation, Moves::Reversal)
			})
		),
		Pokemon<generation>(
			Species::Mew,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Synchronize,
			CombinedStats<generation>{
				Nature::Mild,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
				EVs(EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi), EV(0_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Ancient_Power),
				Move(generation, Moves::Ice_Beam),
				Move(generation, Moves::Psychic),
				Move(generation, Moves::Soft_Boiled)
			})
		),
		Pokemon<generation>(
			Species::Zapdos,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Pressure,
			CombinedStats<generation>{
				Nature::Modest,
				IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(30_bi), IV(31_bi), IV(30_bi)),
				EVs(EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi), EV(0_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Drill_Peck),
				Move(generation, Moves::Hidden_Power),
				Move(generation, Moves::Rain_Dance),
				Move(generation, Moves::Thunder)
			})
		)
	}, true);
}

auto expected_netbattle_supremacy_team() -> Team<Generation::two> {
	constexpr auto generation = Generation::two;
	return Team<generation>({
		Pokemon<generation>(
			Species::Raikou,
			Level(100_bi),
			Gender::genderless,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(0_bi), DV(1_bi), DV(2_bi), DV(3_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Crunch),
				Move(generation, Moves::Rest),
				Move(generation, Moves::Sleep_Talk),
				Move(generation, Moves::Thunderbolt)
			})
		),
		Pokemon<generation>(
			Species::Alakazam,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Encore),
				Move(generation, Moves::Psychic),
				Move(generation, Moves::Recover),
				Move(generation, Moves::Thunder_Punch)
			})
		),
		Pokemon<generation>(
			Species::Exeggutor,
			Level(100_bi),
			Gender::male,
			Item::None,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Explosion),
				Move(generation, Moves::Giga_Drain),
				Move(generation, Moves::Psychic),
				Move(generation, Moves::Thief)
			})
		),
		Pokemon<generation>(
			Species::Snorlax,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Double_Edge),
				Move(generation, Moves::Earthquake),
				Move(generation, Moves::Fire_Blast),
				Move(generation, Moves::Self_Destruct)
			})
		),
		Pokemon<generation>(
			Species::Machamp,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(13_bi), DV(13_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Counter),
				Move(generation, Moves::Cross_Chop),
				Move(generation, Moves::Earthquake),
				Move(generation, Moves::Hidden_Power)
			})
		),
		Pokemon<generation>(
			Species::Gengar,
			Level(100_bi),
			Gender::male,
			Item::Leftovers,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				default_evs<generation>
			},
			RegularMoves({
				Move(generation, Moves::Explosion),
				Move(generation, Moves::Ice_Punch),
				Move(generation, Moves::Psychic),
				Move(generation, Moves::Thunderbolt)
			})
		)
	}, true);
}

TEST_CASE("Netbattle team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle.pnb";
	bounded::visit(nb::read_team_file(file_name), []<any_team TeamType>(TeamType const & team) {
		if constexpr (generation_from<TeamType> == Generation::three) {
			CHECK(team == expected_netbattle_team());
		} else {
			CHECK(false);
		}
	});
}

TEST_CASE("Netbattle Supremacy team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle-supremacy.dpnb";
	bounded::visit(nb::read_team_file(file_name), []<any_team TeamType>(TeamType const & team) {
		if constexpr (generation_from<TeamType> == Generation::two) {
			CHECK(team == expected_netbattle_supremacy_team());
		} else {
			CHECK(false);
		}
	});
}

} // namespace
} // namespace technicalmachine
