// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/header.hpp>
#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/test/ps_usage_stats/usage_bytes.hpp>

#include <tm/get_directory.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

namespace technicalmachine {
namespace {

template<Generation generation>
auto make_smallest_team() -> GenerationGeneric<Team> {
	return GenerationGeneric<Team>(Team<generation>({
		{
			Species::Mew,
			Level(1_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(0_bi), DV(0_bi), DV(0_bi), DV(0_bi)),
				OldGenEVs(EV(0_bi), EV(0_bi), EV(0_bi), EV(0_bi), EV(0_bi))
			},
			RegularMoves({
				Move(generation, Moves::Cut)
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
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Body_Slam),
				Move(generation, Moves::Earthquake)
			})
		},
		{
			Species::Pikachu,
			Level(100_bi),
			Gender::genderless,
			Item::None,
			Ability::Honey_Gather,
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Thunderbolt),
				Move(generation, Moves::Thunder)
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
			CombinedStats<generation>{
				Nature::Hardy,
				DVs(DV(15_bi), DV(15_bi), DV(15_bi), DV(15_bi)),
				OldGenEVs(EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi), EV(252_bi))
			},
			RegularMoves({
				Move(generation, Moves::Body_Slam),
				Move(generation, Moves::Earthquake)
			})
		}
	}));
}

TEST_CASE("Serialize smallest non-empty file", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	constexpr auto generation = Generation::one;
	auto const team = make_smallest_team<generation>();
	usage_stats->add(team, weight);
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	correlations->add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, generation, *usage_stats, *correlations);
	auto const expected = smallest_team_bytes(generation);
	CHECK(string_to_bytes(stream.str()) == expected);
}

TEST_CASE("Serialize team with two Pokemon", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	auto const team = make_team_with_two_pokemon();
	usage_stats->add(team, weight);
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	correlations->add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, *correlations);
	auto const expected = containers::concatenate<containers::vector<std::byte>>(
		string_to_bytes(usage_stats_magic_string),
		// Version
		cast_to_bytes(std::uint16_t(0), 2_bi),
		cast_to_bytes(Generation::one, 1_bi),
		// Number of species
		cast_to_bytes(std::uint16_t(2), 2_bi),

		cast_to_bytes(Species::Pikachu, 2_bi),
		// Weight
		cast_to_bytes(1.0, 8_bi),
		// Speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Speed value
		cast_to_bytes(std::uint16_t(278), 2_bi),
		// Speed weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Tauros, 2_bi),
		// Teammate weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Body_Slam, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Earthquake, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),

		cast_to_bytes(Moves::Thunderbolt, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(278), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Tauros, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Body_Slam, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Earthquake, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Thunder, 2_bi),
		cast_to_bytes(1.0, 8_bi),

		cast_to_bytes(Moves::Thunder, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(278), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Tauros, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Body_Slam, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Earthquake, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Thunderbolt, 2_bi),
		cast_to_bytes(1.0, 8_bi),

		cast_to_bytes(Species::Tauros, 2_bi),
		// Weight
		cast_to_bytes(1.0, 8_bi),
		// Speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Speed weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Pikachu, 2_bi),
		// Teammate weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Thunderbolt, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Thunder, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),

		cast_to_bytes(Moves::Body_Slam, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Pikachu, 2_bi),
		// Teammate weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Thunderbolt, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Thunder, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Earthquake, 2_bi),
		cast_to_bytes(1.0, 8_bi),

		cast_to_bytes(Moves::Earthquake, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Species::Pikachu, 2_bi),
		// Teammate weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),
		cast_to_bytes(Moves::Thunderbolt, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		cast_to_bytes(Moves::Thunder, 2_bi),
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Body_Slam, 2_bi),
		cast_to_bytes(1.0, 8_bi)
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
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	for (auto const team : teams) {
		correlations->add(team, weight);
	}
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, *correlations);
	auto const expected = containers::concatenate<containers::vector<std::byte>>(
		string_to_bytes(usage_stats_magic_string),
		// Version
		cast_to_bytes(std::uint16_t(0), 2_bi),
		cast_to_bytes(Generation::one, 1_bi),
		// Number of species
		cast_to_bytes(std::uint16_t(2), 2_bi),

		cast_to_bytes(Species::Tauros, 2_bi),
		// Weight
		cast_to_bytes(0.5, 8_bi),
		// Speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Speed weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate count
		cast_to_bytes(std::uint16_t(0), 2_bi),
		// Moves count
		cast_to_bytes(std::uint16_t(2), 2_bi),

		cast_to_bytes(Moves::Body_Slam, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(0), 2_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Earthquake, 2_bi),
		cast_to_bytes(1.0, 8_bi),

		cast_to_bytes(Moves::Earthquake, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(318), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(0), 2_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		cast_to_bytes(Moves::Body_Slam, 2_bi),
		cast_to_bytes(1.0, 8_bi),

		cast_to_bytes(Species::Mew, 2_bi),
		// Weight
		cast_to_bytes(0.5, 8_bi),
		// Speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Speed value
		cast_to_bytes(std::uint16_t(7), 2_bi),
		// Speed weight
		cast_to_bytes(1.0, 8_bi),
		// Teammate count
		cast_to_bytes(std::uint16_t(0), 2_bi),
		// Moves count
		cast_to_bytes(std::uint16_t(1), 2_bi),

		cast_to_bytes(Moves::Cut, 2_bi),
		// Move weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, speed count
		cast_to_bytes(std::uint16_t(1), 2_bi),
		// Detailed stats, speed value
		cast_to_bytes(std::uint16_t(7), 2_bi),
		// Detailed stats, speed weight
		cast_to_bytes(1.0, 8_bi),
		// Detailed stats, teammates count
		cast_to_bytes(std::uint16_t(0), 2_bi),
		// Detailed stats, other moves count
		cast_to_bytes(std::uint16_t(0), 2_bi)
	);
	CHECK(string_to_bytes(stream.str()) == expected);
}

TEST_CASE("Serialize smallest non-empty Generation 2 file", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	constexpr auto generation = Generation::two;
	auto const team = make_smallest_team<generation>();
	usage_stats->add(team, weight);
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	correlations->add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, generation, *usage_stats, *correlations);
	auto const expected = smallest_team_bytes(generation);
	CHECK(string_to_bytes(stream.str()) == expected);
}

} // namespace
} // namespace technicalmachine
