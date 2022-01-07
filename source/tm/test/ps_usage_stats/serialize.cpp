// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/get_directory.hpp>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto make_smallest_team() -> GenerationGeneric<Team> {
	constexpr auto generation = Generation::one;
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

TEST_CASE("Serialize smallest non-empty file", "[ps_usage_stats]") {
	auto const path = get_test_directory() / "ps_usage_stats" / "stats.json";
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	auto const team = make_smallest_team();
	usage_stats->add(team, weight);
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	correlations->add(team, weight);
	auto serialized = ps_usage_stats::serialize(Generation::one, *usage_stats, *correlations);
	auto const expected = R"(
		{
			"Pokemon": {
				"Mew": {
					"Moves": {
						"Cut": {
							"Moves": {},
							"Teammates": {},
							"Usage": 1.0
						}
					},
					"Speed": {
						"7": 1.0
					},
					"Usage": 1.0
				}
			},
			"Total teams": 1.0
		}
	)"_json;
	CHECK(serialized == expected);
	std::filesystem::remove(path);
}

} // namespace
} // namespace technicalmachine
