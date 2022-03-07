// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/ps_usage_stats/serialize.hpp>

#include <tm/ps_usage_stats/usage_stats.hpp>

#include <tm/get_directory.hpp>

#include <nlohmann/json.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

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
	auto const team = make_smallest_team();
	usage_stats->add(team, weight);
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	correlations->add(team, weight);
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, *correlations);
	auto const expected = nlohmann::json::parse(R"(
		{
			"Pokemon": {
				"Mew": {
					"Moves": {
						"Cut": {
							"Moves": {},
							"Speed": {
								"7": 1.0
							},
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
	)");
	INFO(stream.str());
	CHECK(nlohmann::json::parse(stream.str()) == expected);
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
	auto const expected = nlohmann::json::parse(R"(
		{
			"Pokemon": {
				"Pikachu": {
					"Moves": {
						"Thunderbolt": {
							"Moves": {
								"Thunder": 1.0
							},
							"Speed": {
								"278": 1.0
							},
							"Teammates": {
								"Tauros": {
									"Moves": {
										"Body Slam": 1.0,
										"Earthquake": 1.0
									},
									"Usage": 1.0
								}
							},
							"Usage": 1.0
						},
						"Thunder": {
							"Moves": {
								"Thunderbolt": 1.0
							},
							"Speed": {
								"278": 1.0
							},
							"Teammates": {
								"Tauros": {
									"Moves": {
										"Body Slam": 1.0,
										"Earthquake": 1.0
									},
									"Usage": 1.0
								}
							},
							"Usage": 1.0
						}
					},
					"Speed": {
						"278": 1.0
					},
					"Usage": 1.0
				},
				"Tauros": {
					"Moves": {
						"Body Slam": {
							"Moves": {
								"Earthquake": 1.0
							},
							"Speed": {
								"318": 1.0
							},
							"Teammates": {
								"Pikachu": {
									"Moves": {
										"Thunderbolt": 1.0,
										"Thunder": 1.0
									},
									"Usage": 1.0
								}
							},
							"Usage": 1.0
						},
						"Earthquake": {
							"Moves": {
								"Body Slam": 1.0
							},
							"Speed": {
								"318": 1.0
							},
							"Teammates": {
								"Pikachu": {
									"Moves": {
										"Thunderbolt": 1.0,
										"Thunder": 1.0
									},
									"Usage": 1.0
								}
							},
							"Usage": 1.0
						}
					},
					"Speed": {
						"318": 1.0
					},
					"Usage": 1.0
				}
			},
			"Total teams": 1.0
		}
	)");
	INFO(stream.str());
	CHECK(nlohmann::json::parse(stream.str()) == expected);
}

TEST_CASE("Serialize two teams", "[ps_usage_stats]") {
	auto usage_stats = std::make_unique<ps_usage_stats::UsageStats>();
	constexpr auto weight = 1.0;
	auto teams = containers::array{make_smallest_team(), make_second_team()};
	for (auto const team : teams) {
		usage_stats->add(team, weight);
	}
	auto correlations = std::make_unique<ps_usage_stats::Correlations>(*usage_stats);
	for (auto const team : teams) {
		correlations->add(team, weight);
	}
	auto stream = std::stringstream();
	ps_usage_stats::serialize(stream, Generation::one, *usage_stats, *correlations);
	auto const expected = nlohmann::json::parse(R"(
		{
			"Pokemon": {
				"Mew": {
					"Moves": {
						"Cut": {
							"Moves": {},
							"Speed": {
								"7": 1.0
							},
							"Teammates": {},
							"Usage": 1.0
						}
					},
					"Speed": {
						"7": 1.0
					},
					"Usage": 0.5
				},
				"Tauros": {
					"Moves": {
						"Body Slam": {
							"Moves": {
								"Earthquake": 1.0
							},
							"Speed": {
								"318": 1.0
							},
							"Teammates": {},
							"Usage": 1.0
						},
						"Earthquake": {
							"Moves": {
								"Body Slam": 1.0
							},
							"Speed": {
								"318": 1.0
							},
							"Teammates": {},
							"Usage": 1.0
						}
					},
					"Speed": {
						"318": 1.0
					},
					"Usage": 0.5
				}
			},
			"Total teams": 2.0
		}
	)");
	INFO(stream.str());
	CHECK(nlohmann::json::parse(stream.str()) == expected);
}

} // namespace
} // namespace technicalmachine
