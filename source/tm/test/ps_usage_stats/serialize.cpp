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

struct WeightedTeam {
	GenerationGeneric<Team> team;
	double weight;
};

struct SerializeInputs {
	explicit SerializeInputs(auto const & ... weighted_teams):
		usage_stats(make_usage_stats(weighted_teams...)),
		correlations(usage_stats)
	{
		populate(correlations, weighted_teams...);
	}

	ps_usage_stats::UsageStats usage_stats;
	ps_usage_stats::Correlations correlations;

private:
	static auto populate(auto & target, auto const & ... weighted_teams) -> void {
		(..., target.add(weighted_teams.team, weighted_teams.weight));
	}
	static auto make_usage_stats(auto const & ... weighted_teams) {
		auto result = ps_usage_stats::UsageStats();
		populate(result, weighted_teams...);
		return result;
	}
};


auto make_smallest_team(double const weight) -> WeightedTeam {
	constexpr auto generation = Generation::one;
	return WeightedTeam{
		GenerationGeneric<Team>(Team<generation>({
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
		})),
		weight
	};
}

TEST_CASE("Serialize smallest non-empty file", "[ps_usage_stats]") {
	auto const path = get_test_directory() / "ps_usage_stats" / "stats.json";
	auto const serialize_inputs = std::make_unique<SerializeInputs>(make_smallest_team(1.0));
	auto serialized = ps_usage_stats::serialize(Generation::one, serialize_inputs->usage_stats, serialize_inputs->correlations);
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
