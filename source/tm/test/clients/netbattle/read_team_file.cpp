// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.clients.nb.read_team_file;

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.known_pokemon;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.iv;
import tm.stat.nature;

import tm.string_conversions.pokemon;
import tm.string_conversions.team;

import tm.ability;
import tm.any_team;
import tm.gender;
import tm.generation;
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
using namespace std::string_view_literals;

constexpr auto expected_netbattle_team = []() -> KnownTeam<Generation::three> {
	constexpr auto generation = Generation::three;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Forretress,
			"Alcatraz"sv,
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
			"Crunch Time"sv,
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
			"Clyde"sv,
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
			"Blinky"sv,
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
			"Houdini"sv,
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
			"Sparky"sv,
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
}();

constexpr auto expected_netbattle_supremacy_team = []() -> KnownTeam<Generation::two> {
	constexpr auto generation = Generation::two;
	return KnownTeam<generation>({
		KnownPokemon<generation>(
			Species::Raikou,
			"Aaron Carter"sv,
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
			"Jackie Chan"sv,
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
			"Old Godzilla"sv,
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
			"Shaq Fu"sv,
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
			"Chuck Norris"sv,
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
			"Abe Lincoln"sv,
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
}();

// std::embed(get_test_directory() / "teams/netbattle.pnb")
constexpr auto netbattle_bytes = containers::array{
	std::byte(0x20), std::byte(0x50), std::byte(0x4E), std::byte(0x42),
	std::byte(0x34), std::byte(0x2E), std::byte(0x31), std::byte(0x0A),
	std::byte(0x4F), std::byte(0x62), std::byte(0x69), std::byte(0x32),
	std::byte(0x4B), std::byte(0x65), std::byte(0x6E), std::byte(0x6F),
	std::byte(0x62), std::byte(0x69), std::byte(0x5F), std::byte(0x49),
	std::byte(0x27), std::byte(0x6C), std::byte(0x6C), std::byte(0x20),
	std::byte(0x70), std::byte(0x72), std::byte(0x6F), std::byte(0x62),
	std::byte(0x61), std::byte(0x62), std::byte(0x6C), std::byte(0x79),
	std::byte(0x20), std::byte(0x6C), std::byte(0x6F), std::byte(0x73),
	std::byte(0x65), std::byte(0x2C), std::byte(0x20), std::byte(0x75),
	std::byte(0x6E), std::byte(0x6C), std::byte(0x65), std::byte(0x73),
	std::byte(0x73), std::byte(0x20), std::byte(0x49), std::byte(0x20),
	std::byte(0x77), std::byte(0x69), std::byte(0x6E), std::byte(0x2E),
	std::byte(0x0D), std::byte(0x0A), std::byte(0x28), std::byte(0x46),
	std::byte(0x69), std::byte(0x72), std::byte(0x73), std::byte(0x74),
	std::byte(0x20), std::byte(0x74), std::byte(0x65), std::byte(0x61),
	std::byte(0x6D), std::byte(0x2C), std::byte(0x20), std::byte(0x49),
	std::byte(0x20), std::byte(0x68), std::byte(0x61), std::byte(0x76),
	std::byte(0x65), std::byte(0x20), std::byte(0x61), std::byte(0x20),
	std::byte(0x4D), std::byte(0x65), std::byte(0x77), std::byte(0x2C),
	std::byte(0x20), std::byte(0x62), std::byte(0x75), std::byte(0x74),
	std::byte(0x20), std::byte(0x49), std::byte(0x20), std::byte(0x61),
	std::byte(0x6C), std::byte(0x73), std::byte(0x6F), std::byte(0x20),
	std::byte(0x68), std::byte(0x61), std::byte(0x76), std::byte(0x65),
	std::byte(0x20), std::byte(0x4A), std::byte(0x6F), std::byte(0x6C),
	std::byte(0x6C), std::byte(0x79), std::byte(0x20), std::byte(0x46),
	std::byte(0x6F), std::byte(0x72), std::byte(0x72), std::byte(0x65),
	std::byte(0x74), std::byte(0x72), std::byte(0x65), std::byte(0x73),
	std::byte(0x73), std::byte(0x29), std::byte(0x17), std::byte(0x49),
	std::byte(0x20), std::byte(0x6D), std::byte(0x75), std::byte(0x73),
	std::byte(0x74), std::byte(0x20), std::byte(0x62), std::byte(0x65),
	std::byte(0x20), std::byte(0x72), std::byte(0x65), std::byte(0x61),
	std::byte(0x6C), std::byte(0x6C), std::byte(0x79), std::byte(0x20),
	std::byte(0x6C), std::byte(0x75), std::byte(0x63), std::byte(0x6B),
	std::byte(0x79), std::byte(0x2E), std::byte(0x09), std::byte(0x59),
	std::byte(0x6F), std::byte(0x75), std::byte(0x20), std::byte(0x72),
	std::byte(0x75), std::byte(0x6C), std::byte(0x65), std::byte(0x21),
	std::byte(0x03), std::byte(0x04), std::byte(0x03), std::byte(0x41),
	std::byte(0x6C), std::byte(0x63), std::byte(0x61), std::byte(0x74),
	std::byte(0x72), std::byte(0x61), std::byte(0x7A), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x66), std::byte(0xBC),
	std::byte(0x85), std::byte(0x5A), std::byte(0x04), std::byte(0x03),
	std::byte(0x71), std::byte(0xE2), std::byte(0x79), std::byte(0x95),
	std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xFA),
	std::byte(0xC2), std::byte(0xA2), std::byte(0xA7), std::byte(0xE0),
	std::byte(0x00), std::byte(0x00), std::byte(0x43), std::byte(0x72),
	std::byte(0x75), std::byte(0x6E), std::byte(0x63), std::byte(0x68),
	std::byte(0x20), std::byte(0x54), std::byte(0x69), std::byte(0x6D),
	std::byte(0x65), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x72), std::byte(0xBC), std::byte(0x8E),
	std::byte(0xDD), std::byte(0x04), std::byte(0x02), std::byte(0x51),
	std::byte(0xD9), std::byte(0x01), std::byte(0x4B), std::byte(0xFF),
	std::byte(0xFF), std::byte(0xFF), std::byte(0xF8), std::byte(0x07),
	std::byte(0xE0), std::byte(0x07), std::byte(0xE0), std::byte(0x20),
	std::byte(0x00), std::byte(0x43), std::byte(0x6C), std::byte(0x79),
	std::byte(0x64), std::byte(0x65), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x2F), std::byte(0x3C), std::byte(0x85), std::byte(0x54),
	std::byte(0x04), std::byte(0x03), std::byte(0x43), std::byte(0x09),
	std::byte(0xCD), std::byte(0x0D), std::byte(0xF0), std::byte(0x7F),
	std::byte(0xFF), std::byte(0xF8), std::byte(0x00), std::byte(0x02),
	std::byte(0xA7), std::byte(0xE0), std::byte(0x05), std::byte(0x60),
	std::byte(0x42), std::byte(0x6C), std::byte(0x69), std::byte(0x6E),
	std::byte(0x6B), std::byte(0x79), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x6B),
	std::byte(0x3C), std::byte(0x8E), std::byte(0xDA), std::byte(0x04),
	std::byte(0x03), std::byte(0xB2), std::byte(0x21), std::byte(0xE1),
	std::byte(0x4B), std::byte(0xFF), std::byte(0xFF), std::byte(0xFF),
	std::byte(0xF9), std::byte(0xC4), std::byte(0x00), std::byte(0x07),
	std::byte(0xE0), std::byte(0x02), std::byte(0x40), std::byte(0x48),
	std::byte(0x6F), std::byte(0x75), std::byte(0x64), std::byte(0x69),
	std::byte(0x6E), std::byte(0x69), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x4B), std::byte(0xBC),
	std::byte(0x85), std::byte(0x60), std::byte(0x04), std::byte(0x00),
	std::byte(0x73), std::byte(0x12), std::byte(0x61), std::byte(0x89),
	std::byte(0xFF), std::byte(0xFF), std::byte(0xFF), std::byte(0xF8),
	std::byte(0x00), std::byte(0x20), std::byte(0x07), std::byte(0xE7),
	std::byte(0xE0), std::byte(0x00), std::byte(0x53), std::byte(0x70),
	std::byte(0x61), std::byte(0x72), std::byte(0x6B), std::byte(0x79),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x48), std::byte(0xBC), std::byte(0x85),
	std::byte(0x5E), std::byte(0x04), std::byte(0x03), std::byte(0x52),
	std::byte(0xDA), std::byte(0x75), std::byte(0xCD), std::byte(0xFF),
	std::byte(0xFF), std::byte(0xDE), std::byte(0xF8), std::byte(0x00),
	std::byte(0x20), std::byte(0x07), std::byte(0xE7), std::byte(0xE0),
	std::byte(0x00)
};

template<any_known_team ExpectedTeam>
constexpr auto matches(auto const & source, ExpectedTeam const expected) -> bool {
	return tv::visit(nb::read_team_file(source), tv::overload(
		[&](ExpectedTeam const & parsed) { return parsed == expected; },
		[](auto const &) { return false; }
	));
}

static_assert(matches(netbattle_bytes, expected_netbattle_team));

TEST_CASE("Netbattle team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle.pnb";
	CHECK(matches(netbattle_bytes, expected_netbattle_team));
	CHECK(matches(file_name, expected_netbattle_team));
}

TEST_CASE("Netbattle Supremacy team", "[Netbattle]") {
	auto const file_name = get_test_directory() / "teams/netbattle-supremacy.dpnb";
	CHECK(matches(file_name, expected_netbattle_supremacy_team));
}

} // namespace
} // namespace technicalmachine
