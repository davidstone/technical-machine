// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/generation.hpp>

#include <tm/team_predictor/estimate.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/test/ps_usage_stats/usage_bytes.hpp>

#include <containers/algorithms/accumulate.hpp>
#include <containers/algorithms/transform.hpp>

#include <sstream>

#include <catch2/catch_test_macros.hpp>

namespace technicalmachine {
namespace {

auto make_usage_stats(std::span<std::byte const> input_bytes) {
	return UsageStats::make(std::stringstream(std::string(
		reinterpret_cast<char const *>(containers::data(input_bytes)),
		static_cast<std::size_t>(containers::size(input_bytes))
	)));
}

TEST_CASE("Smallest generation 1 team", "[UsageStats]") {
	auto const usage_stats = make_usage_stats(smallest_team_bytes(Generation::one));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<Moves, float>({{Moves::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>());
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>());
}

TEST_CASE("Smallest generation 2 team", "[UsageStats]") {
	auto const usage_stats = make_usage_stats(smallest_team_bytes(Generation::two));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<Moves, float>({{Moves::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>({{Item::None, 1.0}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>());
}

TEST_CASE("Smallest generation 3 team", "[UsageStats]") {
	auto const usage_stats = make_usage_stats(smallest_team_bytes(Generation::three));
	auto const & base = usage_stats.assuming();
	auto const & map = base.map();
	CHECK(containers::size(map) == 1_bi);
	auto const ptr = base(Species::Mew);
	REQUIRE(ptr);
	CHECK(ptr->moves.map() == containers::flat_map<Moves, float>({{Moves::Cut, 1.0}}));
	CHECK(ptr->items.map() == containers::flat_map<Item, float>({{Item::None, 1.0}}));
	CHECK(ptr->abilities.map() == containers::flat_map<Ability, float>({{Ability::Synchronize, 1.0}}));
}

} // namespace
} // namespace technicalmachine
