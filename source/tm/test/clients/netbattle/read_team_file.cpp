// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

import tm.clients.nb.read_team_file;

import tm.move.move_name;

import tm.pokemon.initial_pokemon;
import tm.pokemon.level;
import tm.pokemon.species;

import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;
import tm.stat.stat_style;

import tm.ability;
import tm.gender;
import tm.get_directory;
import tm.initial_team;
import tm.item;

import bounded;
import containers;
import tv;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;
using namespace std::string_view_literals;

constexpr auto expected_netbattle_team = InitialTeam<SpecialStyle::split>({
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Forretress,
		.nickname = "Alcatraz"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.ability = Ability::Sturdy,
		.stats = {
			.nature = Nature::Jolly,
			.evs = EVs(EV(88_bi), EV(84_bi), EV(84_bi), EV(0_bi), EV(0_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Earthquake,
			MoveName::Explosion,
			MoveName::Rapid_Spin,
			MoveName::Spikes,
		}}
	},
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Houndoom,
		.nickname = "Crunch Time"sv,
		.gender = Gender::male,
		.item = Item::Salac_Berry,
		.ability = Ability::Flash_Fire,
		.stats = {
			.nature = Nature::Naive,
			.evs = EVs(EV(0_bi), EV(252_bi), EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Crunch,
			MoveName::Endure,
			MoveName::Fire_Blast,
			MoveName::Reversal,
		}}
	},
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Gengar,
		.nickname = "Clyde"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.ability = Ability::Levitate,
		.stats = {
			.nature = Nature::Timid,
			.dvs_or_ivs = IVs(IV(31_bi), IV(0_bi), IV(31_bi), IV(31_bi), IV(31_bi), IV(31_bi)),
			.evs = EVs(EV(0_bi), EV(0_bi), EV(84_bi), EV(0_bi), EV(172_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Dream_Eater,
			MoveName::Hypnosis,
			MoveName::Mean_Look,
			MoveName::Nightmare,
		}}
	},
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Heracross,
		.nickname = "Blinky"sv,
		.gender = Gender::male,
		.item = Item::Salac_Berry,
		.ability = Ability::Swarm,
		.stats = {
			.nature = Nature::Jolly,
			.evs = EVs(EV(56_bi), EV(128_bi), EV(0_bi), EV(0_bi), EV(72_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Endure,
			MoveName::Flail,
			MoveName::Megahorn,
			MoveName::Reversal,
		}}
	},
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Mew,
		.nickname = "Houdini"sv,
		.item = Item::Leftovers,
		.ability = Ability::Synchronize,
		.stats = {
			.nature = Nature::Mild,
			.evs = EVs(EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi), EV(0_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Ancient_Power,
			MoveName::Ice_Beam,
			MoveName::Psychic,
			MoveName::Soft_Boiled,
		}}
	},
	InitialPokemon<SpecialStyle::split>{
		.species = Species::Zapdos,
		.nickname = "Sparky"sv,
		.item = Item::Leftovers,
		.ability = Ability::Pressure,
		.stats = {
			.nature = Nature::Modest,
			.dvs_or_ivs = IVs(IV(31_bi), IV(31_bi), IV(31_bi), IV(30_bi), IV(31_bi), IV(30_bi)),
			.evs = EVs(EV(0_bi), EV(4_bi), EV(0_bi), EV(252_bi), EV(0_bi), EV(252_bi)),
		},
		.moves = {{
			MoveName::Drill_Peck,
			MoveName::Hidden_Power,
			MoveName::Rain_Dance,
			MoveName::Thunder,
		}}
	},
});

constexpr auto expected_netbattle_supremacy_team = InitialTeam<SpecialStyle::combined>({
	{
		.species = Species::Raikou,
		.nickname = "Aaron Carter"sv,
		.item = Item::Leftovers,
		.stats = {
			.dvs_or_ivs = DVs(DV(0_bi), DV(1_bi), DV(2_bi), DV(3_bi)),
		},
		.moves = {{
			MoveName::Crunch,
			MoveName::Rest,
			MoveName::Sleep_Talk,
			MoveName::Thunderbolt,
		}}
	},
	{
		.species = Species::Alakazam,
		.nickname = "Jackie Chan"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.moves = {{
			MoveName::Encore,
			MoveName::Psychic,
			MoveName::Recover,
			MoveName::Thunder_Punch,
		}}
	},
	{
		.species = Species::Exeggutor,
		.nickname = "Old Godzilla"sv,
		.gender = Gender::male,
		.moves = {{
			MoveName::Explosion,
			MoveName::Giga_Drain,
			MoveName::Psychic,
			MoveName::Thief,
		}}
	},
	{
		.species = Species::Snorlax,
		.nickname = "Shaq Fu"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.moves = {{
			MoveName::Double_Edge,
			MoveName::Earthquake,
			MoveName::Fire_Blast,
			MoveName::Self_Destruct,
		}}
	},
	{
		.species = Species::Machamp,
		.nickname = "Chuck Norris"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.stats = {
			.dvs_or_ivs = DVs(DV(13_bi), DV(13_bi), DV(15_bi), DV(15_bi)),
		},
		.moves = {{
			MoveName::Counter,
			MoveName::Cross_Chop,
			MoveName::Earthquake,
			MoveName::Hidden_Power,
		}}
	},
	{
		.species = Species::Gengar,
		.nickname = "Abe Lincoln"sv,
		.gender = Gender::male,
		.item = Item::Leftovers,
		.moves = {{
			MoveName::Explosion,
			MoveName::Ice_Punch,
			MoveName::Psychic,
			MoveName::Thunderbolt,
		}}
	},
});

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

constexpr auto netbattle_supremacy_bytes = containers::array{
	std::byte(0x20), std::byte(0x50), std::byte(0x4E), std::byte(0x42),
	std::byte(0x35), std::byte(0x2E), std::byte(0x30), std::byte(0x09),
	std::byte(0x4F), std::byte(0x62), std::byte(0x69), std::byte(0x20),
	std::byte(0x28), std::byte(0x47), std::byte(0x53), std::byte(0x43),
	std::byte(0x29), std::byte(0x26), std::byte(0x54), std::byte(0x68),
	std::byte(0x65), std::byte(0x20), std::byte(0x55), std::byte(0x6C),
	std::byte(0x74), std::byte(0x69), std::byte(0x6D), std::byte(0x61),
	std::byte(0x74), std::byte(0x65), std::byte(0x20), std::byte(0x54),
	std::byte(0x65), std::byte(0x61), std::byte(0x6D), std::byte(0x20),
	std::byte(0x6F), std::byte(0x66), std::byte(0x20), std::byte(0x55),
	std::byte(0x6C), std::byte(0x74), std::byte(0x69), std::byte(0x6D),
	std::byte(0x61), std::byte(0x74), std::byte(0x65), std::byte(0x20),
	std::byte(0x44), std::byte(0x65), std::byte(0x73), std::byte(0x74),
	std::byte(0x69), std::byte(0x6E), std::byte(0x79), std::byte(0x2E),
	std::byte(0x36), std::byte(0x59), std::byte(0x6F), std::byte(0x75),
	std::byte(0x27), std::byte(0x72), std::byte(0x65), std::byte(0x20),
	std::byte(0x73), std::byte(0x74), std::byte(0x72), std::byte(0x6F),
	std::byte(0x6E), std::byte(0x67), std::byte(0x2E), std::byte(0x20),
	std::byte(0x42), std::byte(0x75), std::byte(0x74), std::byte(0x20),
	std::byte(0x74), std::byte(0x68), std::byte(0x65), std::byte(0x6E),
	std::byte(0x20), std::byte(0x61), std::byte(0x67), std::byte(0x61),
	std::byte(0x69), std::byte(0x6E), std::byte(0x2C), std::byte(0x20),
	std::byte(0x73), std::byte(0x6D), std::byte(0x65), std::byte(0x6C),
	std::byte(0x6C), std::byte(0x20), std::byte(0x69), std::byte(0x73),
	std::byte(0x6E), std::byte(0x27), std::byte(0x74), std::byte(0x20),
	std::byte(0x65), std::byte(0x76), std::byte(0x65), std::byte(0x72),
	std::byte(0x79), std::byte(0x74), std::byte(0x68), std::byte(0x69),
	std::byte(0x6E), std::byte(0x67), std::byte(0x2E), std::byte(0x00),
	std::byte(0x13), std::byte(0x49), std::byte(0x20), std::byte(0x62),
	std::byte(0x6C), std::byte(0x61), std::byte(0x6D), std::byte(0x65),
	std::byte(0x20), std::byte(0x41), std::byte(0x6C), std::byte(0x2D),
	std::byte(0x4A), std::byte(0x61), std::byte(0x7A), std::byte(0x65),
	std::byte(0x65), std::byte(0x72), std::byte(0x61), std::byte(0x2E),
	std::byte(0x01), std::byte(0x01), std::byte(0x03), std::byte(0x41),
	std::byte(0x61), std::byte(0x72), std::byte(0x6F), std::byte(0x6E),
	std::byte(0x20), std::byte(0x43), std::byte(0x61), std::byte(0x72),
	std::byte(0x74), std::byte(0x65), std::byte(0x72), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x79), std::byte(0x9C),
	std::byte(0x85), std::byte(0x5E), std::byte(0x00), std::byte(0x02),
	std::byte(0x55), std::byte(0x1A), std::byte(0xF9), std::byte(0xD0),
	std::byte(0x50), std::byte(0x04), std::byte(0x43), std::byte(0x00),
	std::byte(0x20), std::byte(0x00), std::byte(0x07), std::byte(0xE7),
	std::byte(0xE0), std::byte(0x00), std::byte(0x4A), std::byte(0x61),
	std::byte(0x63), std::byte(0x6B), std::byte(0x69), std::byte(0x65),
	std::byte(0x20), std::byte(0x43), std::byte(0x68), std::byte(0x61),
	std::byte(0x6E), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x9C), std::byte(0x85),
	std::byte(0x40), std::byte(0x00), std::byte(0x03), std::byte(0xA4),
	std::byte(0xC2), std::byte(0x85), std::byte(0xD2), std::byte(0xF7),
	std::byte(0xBD), std::byte(0xEF), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x4F), std::byte(0x6C), std::byte(0x64),
	std::byte(0x20), std::byte(0x47), std::byte(0x6F), std::byte(0x64),
	std::byte(0x7A), std::byte(0x69), std::byte(0x6C), std::byte(0x6C),
	std::byte(0x61), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x33), std::byte(0x9C), std::byte(0x80), std::byte(0x00),
	std::byte(0x00), std::byte(0x03), std::byte(0xC2), std::byte(0x82),
	std::byte(0x61), std::byte(0xC8), std::byte(0xF7), std::byte(0xBD),
	std::byte(0xEF), std::byte(0x00), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00),
	std::byte(0x53), std::byte(0x68), std::byte(0x61), std::byte(0x71),
	std::byte(0x20), std::byte(0x46), std::byte(0x75), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x20), std::byte(0x47),
	std::byte(0x9C), std::byte(0x85), std::byte(0x40), std::byte(0x00),
	std::byte(0x03), std::byte(0x01), std::byte(0xB9), std::byte(0x01),
	std::byte(0x68), std::byte(0xF7), std::byte(0xBD), std::byte(0xEF),
	std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x43),
	std::byte(0x68), std::byte(0x75), std::byte(0x63), std::byte(0x6B),
	std::byte(0x20), std::byte(0x4E), std::byte(0x6F), std::byte(0x72),
	std::byte(0x72), std::byte(0x69), std::byte(0x73), std::byte(0x20),
	std::byte(0x20), std::byte(0x20), std::byte(0x22), std::byte(0x1C),
	std::byte(0x85), std::byte(0x40), std::byte(0x00), std::byte(0x02),
	std::byte(0x21), std::byte(0x20), std::byte(0xDC), std::byte(0xB6),
	std::byte(0xF6), std::byte(0xB5), std::byte(0xEF), std::byte(0x00),
	std::byte(0x00), std::byte(0x00), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x00), std::byte(0x41), std::byte(0x62),
	std::byte(0x65), std::byte(0x20), std::byte(0x4C), std::byte(0x69),
	std::byte(0x6E), std::byte(0x63), std::byte(0x6F), std::byte(0x6C),
	std::byte(0x6E), std::byte(0x20), std::byte(0x20), std::byte(0x20),
	std::byte(0x20), std::byte(0x2F), std::byte(0x1C), std::byte(0x85),
	std::byte(0x5E), std::byte(0x00), std::byte(0x03), std::byte(0xC3),
	std::byte(0x1A), std::byte(0x61), std::byte(0xD0), std::byte(0xF7),
	std::byte(0xBD), std::byte(0xEF), std::byte(0x00), std::byte(0x00),
	std::byte(0x00), std::byte(0x07), std::byte(0xE7), std::byte(0xE0),
	std::byte(0x20)
};

static_assert(nb::read_team_file(netbattle_bytes) == expected_netbattle_team);
static_assert(nb::read_team_file(netbattle_supremacy_bytes) == expected_netbattle_supremacy_team);

} // namespace
} // namespace technicalmachine
