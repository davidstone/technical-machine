// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.level;
import tm.pokemon.species;

import tm.ps_usage_stats.serialize;

import tm.ps_usage_stats.header;
import tm.ps_usage_stats.usage_stats;

import tm.stat.combined_stats;
import tm.stat.ev;
import tm.stat.evs;
import tm.stat.iv;
import tm.stat.nature;

import tm.test.usage_bytes;

import tm.ability;
import tm.gender;
import tm.generation;
import tm.get_directory;
import tm.item;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

template<Generation generation>
auto make_smallest_team() -> GenerationGeneric<Team> {
	return GenerationGeneric<Team>(Team<generation>({
		{
			Species::Mew,
			Level(1_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStatsFor<generation>{
				Nature::Hardy,
				DVs(DV(0_bi), DV(0_bi), DV(0_bi), DV(0_bi)),
				OldGenEVs(EV(0_bi), EV(0_bi), EV(0_bi), EV(0_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Cut)
			})
		}
	}));
}

auto make_team_with_two_pokemon() -> GenerationGeneric<Team> {
	constexpr auto generation = Generation::one;
	return GenerationGeneric<Team>(Team<generation>({
		{
			Species::Tauros,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStatsFor<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Body_Slam),
				Move(generation, MoveName::Earthquake)
			})
		},
		{
			Species::Pikachu,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStatsFor<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Thunderbolt),
				Move(generation, MoveName::Thunder)
			})
		}
	}));
}

auto make_second_team() -> GenerationGeneric<Team> {
	constexpr auto generation = Generation::one;
	return GenerationGeneric<Team>(Team<generation>({
		{
			Species::Tauros,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStatsFor<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, MoveName::Body_Slam),
				Move(generation, MoveName::Earthquake)
			})
		}
	}));
}

auto string_to_bytes(std::string_view const str) {
	return containers::vector(std::as_bytes(std::span(str)));
}

TEST_CASE("Serialize smallest non-empty file", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	constexpr auto generation = Generation::one;
	auto const team = make_smallest_team<generation>();
	usage_stats->add(team, weight);
	auto correlations = ps_usage_stats::Correlations(*usage_stats);
	correlations.add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, generation, *usage_stats, correlations);
	auto const expected = smallest_team_bytes(generation);
	CHECK(string_to_bytes(stream.str()) == expected);
}

TEST_CASE("Serialize team with two Pokemon", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	auto const team = make_team_with_two_pokemon();
	usage_stats->add(team, weight);
	auto correlations = ps_usage_stats::Correlations(*usage_stats);
	correlations.add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, correlations);
	auto const expected = containers::concatenate<containers::vector<std::byte>>(
		usage_stats_magic_string,
		version_bytes(0),
		generation_bytes(Generation::one),
		count_bytes(2),
			species_bytes(Species::Pikachu),
				weight_bytes(1.0),
				count_bytes(1),
					speed_bytes(278),
						weight_bytes(1.0),
				count_bytes(1), // Teammates
					species_bytes(Species::Tauros),
						weight_bytes(1.0),
						count_bytes(2),
							move_bytes(MoveName::Body_Slam),
								weight_bytes(1.0),
							move_bytes(MoveName::Earthquake),
								weight_bytes(1.0),
				count_bytes(2), // MoveName
					move_bytes(MoveName::Thunderbolt),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(278),
								weight_bytes(1.0),
						count_bytes(1), // Teammates
							species_bytes(Species::Tauros),
								weight_bytes(1.0),
								count_bytes(2),
									move_bytes(MoveName::Body_Slam),
										weight_bytes(1.0),
									move_bytes(MoveName::Earthquake),
										weight_bytes(1.0),
						count_bytes(1), // Other moves
							move_bytes(MoveName::Thunder),
								weight_bytes(1.0),
					move_bytes(MoveName::Thunder),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(278),
								weight_bytes(1.0),
						count_bytes(1), // Teammates
							species_bytes(Species::Tauros),
								weight_bytes(1.0),
								count_bytes(2), // MoveName
									move_bytes(MoveName::Body_Slam),
										weight_bytes(1.0),
									move_bytes(MoveName::Earthquake),
										weight_bytes(1.0),
							count_bytes(1), // Other moves
								move_bytes(MoveName::Thunderbolt),
									weight_bytes(1.0),
			species_bytes(Species::Tauros),
				weight_bytes(1.0),
				count_bytes(1),
					speed_bytes(318),
						weight_bytes(1.0),
				count_bytes(1), // Teammates
					species_bytes(Species::Pikachu),
						weight_bytes(1.0),
						count_bytes(2), // MoveName
							move_bytes(MoveName::Thunderbolt),
								weight_bytes(1.0),
							move_bytes(MoveName::Thunder),
								weight_bytes(1.0),
				count_bytes(2), // MoveName
					move_bytes(MoveName::Body_Slam),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(318),
								weight_bytes(1.0),
						count_bytes(1), // Teammates
							species_bytes(Species::Pikachu),
								weight_bytes(1.0),
								count_bytes(2), // MoveName
									move_bytes(MoveName::Thunderbolt),
										weight_bytes(1.0),
									move_bytes(MoveName::Thunder),
										weight_bytes(1.0),
						count_bytes(1),
							move_bytes(MoveName::Earthquake),
								weight_bytes(1.0),
					move_bytes(MoveName::Earthquake),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(318),
								weight_bytes(1.0),
						count_bytes(1),
							species_bytes(Species::Pikachu),
								weight_bytes(1.0),
								count_bytes(2),
									move_bytes(MoveName::Thunderbolt),
										weight_bytes(1.0),
									move_bytes(MoveName::Thunder),
										weight_bytes(1.0),
								count_bytes(1),
									move_bytes(MoveName::Body_Slam),
									weight_bytes(1.0)
	);
	CHECK(string_to_bytes(stream.str()) == expected);
}

TEST_CASE("Serialize two teams", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	constexpr auto generation = Generation::one;
	auto teams = containers::array{make_smallest_team<generation>(), make_second_team()};
	for (auto const team : teams) {
		usage_stats->add(team, weight);
	}
	auto correlations = ps_usage_stats::Correlations(*usage_stats);
	for (auto const team : teams) {
		correlations.add(team, weight);
	}
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, correlations);
	auto const expected = containers::concatenate<containers::vector<std::byte>>(
		usage_stats_magic_string,
		version_bytes(0),
		generation_bytes(Generation::one),
		count_bytes(2),
			species_bytes(Species::Tauros),
				weight_bytes(0.5),
				count_bytes(1),
					speed_bytes(318),
						weight_bytes(1.0),
				count_bytes(0), // Teammates
				count_bytes(2),
					move_bytes(MoveName::Body_Slam),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(318),
								weight_bytes(1.0),
						count_bytes(0), // Teammates
						count_bytes(1),
							move_bytes(MoveName::Earthquake),
								weight_bytes(1.0),
					move_bytes(MoveName::Earthquake),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(318),
								weight_bytes(1.0),
						count_bytes(0), // Teammates
						count_bytes(1),
							move_bytes(MoveName::Body_Slam),
								weight_bytes(1.0),
			species_bytes(Species::Mew),
				weight_bytes(0.5),
				count_bytes(1),
					speed_bytes(7),
						weight_bytes(1.0),
				count_bytes(0), // Teammates
				count_bytes(1),
					move_bytes(MoveName::Cut),
						weight_bytes(1.0),
						count_bytes(1),
							speed_bytes(7),
								weight_bytes(1.0),
						count_bytes(0), // Teammates
						count_bytes(0) // MoveName
	);
	CHECK(string_to_bytes(stream.str()) == expected);
}

TEST_CASE("Serialize smallest non-empty Generation 2 file", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	constexpr auto generation = Generation::two;
	auto const team = make_smallest_team<generation>();
	usage_stats->add(team, weight);
	auto correlations = ps_usage_stats::Correlations(*usage_stats);
	correlations.add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, generation, *usage_stats, correlations);
	auto const expected = smallest_team_bytes(generation);
	CHECK(string_to_bytes(stream.str()) == expected);
}

} // namespace
} // namespace technicalmachine
