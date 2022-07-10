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

auto expected_netbattle_team() -> KnownTeam<Generation::three> {
	constexpr auto generation = Generation::three;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Forretress,
			"Alcatraz",
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
				Move(generation, MoveName::Earthquake),
				Move(generation, MoveName::Explosion),
				Move(generation, MoveName::Rapid_Spin),
				Move(generation, MoveName::Spikes)
			})
		),
		KnownPokemon<generation>(
			Species::Houndoom,
			"Crunch Time",
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
				Move(generation, MoveName::Crunch),
				Move(generation, MoveName::Endure),
				Move(generation, MoveName::Fire_Blast),
				Move(generation, MoveName::Reversal)
			})
		),
		KnownPokemon<generation>(
			Species::Gengar,
			"Clyde",
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
				Move(generation, MoveName::Dream_Eater),
				Move(generation, MoveName::Hypnosis),
				Move(generation, MoveName::Mean_Look),
				Move(generation, MoveName::Nightmare)
			})
		),
		KnownPokemon<generation>(
			Species::Heracross,
			"Blinky",
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
				Move(generation, MoveName::Endure),
				Move(generation, MoveName::Flail),
				Move(generation, MoveName::Megahorn),
				Move(generation, MoveName::Reversal)
			})
		),
		KnownPokemon<generation>(
			Species::Mew,
			"Houdini",
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
				Move(generation, MoveName::Ancient_Power),
				Move(generation, MoveName::Ice_Beam),
				Move(generation, MoveName::Psychic),
				Move(generation, MoveName::Soft_Boiled)
			})
		),
		KnownPokemon<generation>(
			Species::Zapdos,
			"Sparky",
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
				Move(generation, MoveName::Drill_Peck),
				Move(generation, MoveName::Hidden_Power),
				Move(generation, MoveName::Rain_Dance),
				Move(generation, MoveName::Thunder)
			})
		)
	});
}

auto expected_netbattle_supremacy_team() -> KnownTeam<Generation::two> {
	constexpr auto generation = Generation::two;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Raikou,
			"Aaron Carter",
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
				Move(generation, MoveName::Crunch),
				Move(generation, MoveName::Rest),
				Move(generation, MoveName::Sleep_Talk),
				Move(generation, MoveName::Thunderbolt)
			})
		),
		KnownPokemon<generation>(
			Species::Alakazam,
			"Jackie Chan",
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
				Move(generation, MoveName::Encore),
				Move(generation, MoveName::Psychic),
				Move(generation, MoveName::Recover),
				Move(generation, MoveName::Thunder_Punch)
			})
		),
		KnownPokemon<generation>(
			Species::Exeggutor,
			"Old Godzilla",
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
				Move(generation, MoveName::Explosion),
				Move(generation, MoveName::Giga_Drain),
				Move(generation, MoveName::Psychic),
				Move(generation, MoveName::Thief)
			})
		),
		KnownPokemon<generation>(
			Species::Snorlax,
			"Shaq Fu",
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
				Move(generation, MoveName::Double_Edge),
				Move(generation, MoveName::Earthquake),
				Move(generation, MoveName::Fire_Blast),
				Move(generation, MoveName::Self_Destruct)
			})
		),
		KnownPokemon<generation>(
			Species::Machamp,
			"Chuck Norris",
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
				Move(generation, MoveName::Counter),
				Move(generation, MoveName::Cross_Chop),
				Move(generation, MoveName::Earthquake),
				Move(generation, MoveName::Hidden_Power)
			})
		),
		KnownPokemon<generation>(
			Species::Gengar,
			"Abe Lincoln",
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
				Move(generation, MoveName::Explosion),
				Move(generation, MoveName::Ice_Punch),
				Move(generation, MoveName::Psychic),
				Move(generation, MoveName::Thunderbolt)
			})
		)
	});
}

TEST_CASE("Netbattle team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle.pnb";
	bounded::visit(nb::read_team_file(file_name), []<any_known_team TeamType>(TeamType const & team) {
		if constexpr (generation_from<TeamType> == Generation::three) {
			CHECK(team == expected_netbattle_team());
		} else {
			CHECK(false);
		}
	});
}

TEST_CASE("Netbattle Supremacy team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle-supremacy.dpnb";
	bounded::visit(nb::read_team_file(file_name), []<any_known_team TeamType>(TeamType const & team) {
		if constexpr (generation_from<TeamType> == Generation::two) {
			CHECK(team == expected_netbattle_supremacy_team());
		} else {
			CHECK(false);
		}
	});
}

} // namespace
} // namespace technicalmachine
