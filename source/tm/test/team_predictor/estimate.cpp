// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

import tm.move.move_name;

import tm.pokemon.species;

import tm.string_conversions.generation;

import tm.team_predictor.estimate;
import tm.team_predictor.usage_stats;

import tm.test.usage_bytes;

import tm.generation;

import containers;
import std_module;

namespace technicalmachine {
namespace {

TEST_CASE("Estimate", "[team_predictor]") {
	for (auto const generation : {Generation::one, Generation::two, Generation::three}) {
		INFO("Generation " << to_string(generation));
		auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(generation));
		auto const ptr = usage_stats.assuming()(Species::Mew);
		REQUIRE(ptr);
		CHECK(containers::sum(containers::transform(ptr->moves.map(), containers::get_mapped)) > 0.0F);
		auto estimate = Estimate(usage_stats);
		CHECK(estimate.probability(Species::Mew) > 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
		estimate.update(usage_stats, Species::Mew);
		CHECK(estimate.probability(Species::Mew) == 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
		estimate.update(usage_stats, Species::Mew, MoveName::Cut);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) == 0.0);
	}
}

TEST_CASE("Estimate seeing unused Pokemon", "[team_predictor]") {
	for (auto const generation : {Generation::one, Generation::two, Generation::three}) {
		INFO("Generation " << to_string(generation));
		auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(generation));
		auto const ptr = usage_stats.assuming()(Species::Mew);
		REQUIRE(ptr);
		auto estimate = Estimate(usage_stats);
		CHECK(estimate.probability(Species::Mew) > 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
		estimate.update(usage_stats, Species::Pikachu);
		CHECK(estimate.probability(Species::Mew) > 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
		estimate.update(usage_stats, Species::Mew);
		CHECK(estimate.probability(Species::Mew) == 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
		estimate.update(usage_stats, Species::Mew, MoveName::Tackle);
		CHECK(estimate.probability(Species::Mew) == 0.0);
		CHECK(estimate.probability(Species::Mew, MoveName::Cut) > 0.0);
	}
}

} // namespace
} // namespace technicalmachine
