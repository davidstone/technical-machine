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

import tm.ability;
import tm.generation;
import tm.item;
import tm.weight;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

TEST_CASE("Smallest generation 1 team", "[UsageStats]") {
	auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(Generation::one));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, Weight<float>>({{MoveName::Cut, Weight(1.0F)}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, Weight<float>>());
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, Weight<float>>());
}

TEST_CASE("Smallest generation 2 team", "[UsageStats]") {
	auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(Generation::two));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, Weight<float>>({{MoveName::Cut, Weight(1.0F)}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, Weight<float>>({{Item::None, Weight(1.0F)}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, Weight<float>>());
}

TEST_CASE("Smallest generation 3 team", "[UsageStats]") {
	auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(Generation::three));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, Weight<float>>({{MoveName::Cut, Weight(1.0F)}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, Weight<float>>({{Item::None, Weight(1.0F)}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, Weight<float>>({{Ability::Synchronize, Weight(1.0F)}}));
}

} // namespace
} // namespace technicalmachine
