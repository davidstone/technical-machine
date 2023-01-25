// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
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
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, float>({{MoveName::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>());
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>());
}

TEST_CASE("Smallest generation 2 team", "[UsageStats]") {
	auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(Generation::two));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, float>({{MoveName::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>({{Item::None, 1.0}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>());
}

TEST_CASE("Smallest generation 3 team", "[UsageStats]") {
	auto const usage_stats = bytes_to_usage_stats(smallest_team_bytes(Generation::three));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<MoveName, float>({{MoveName::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>({{Item::None, 1.0}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>({{Ability::Synchronize, 1.0}}));
}

} // namespace
} // namespace technicalmachine
