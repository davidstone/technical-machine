// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <catch2/catch_test_macros.hpp>

export module tm.test.move.possible_side_effects;

import tm.move.move_name;
import tm.move.side_effects;

import tm.pokemon.species;

import tm.test.pokemon_init;

import tm.environment;
import tm.generation;
import tm.team;

import bounded;
import containers;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

TEST_CASE("Recover", "[possible_side_effects]") {
	constexpr auto generation = Generation::four;
	auto const environment = Environment();
	auto const user = Team<generation>({{
		{
			.species = Species::Starmie,
			.moves = {{
				MoveName::Recover,
			}}
		},
	}});
	auto const other = Team<generation>({{
		{
			.species = Species::Bulbasaur,
			.moves = {{
				MoveName::Tackle,
			}}
		},
	}});

	auto const recover_side_effects = possible_side_effects(
		MoveName::Recover,
		user.pokemon(),
		other,
		environment
	);
	CHECK(containers::size(recover_side_effects) == 1_bi);
}

} // namespace
} // namespace technicalmachine
