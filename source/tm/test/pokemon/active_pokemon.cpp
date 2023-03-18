// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <compare>
#include <catch2/catch_test_macros.hpp>

import tm.move.move;
import tm.move.move_name;
import tm.move.regular_moves;

import tm.pokemon.active_pokemon;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.combined_stats;

import tm.status.clears_status;
import tm.status.status_name;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;

import bounded;

namespace technicalmachine {
namespace {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

constexpr auto regular_moves(auto... moves) {
	return RegularMoves{Move(generation, moves)...};
}

TEST_CASE("Chesto Berry awakens from Rest", "[active_pokemon]") {
	auto pokemon = Pokemon<generation>(
		Species::Charmander,
		Level(100_bi),
		Gender::female,
		Item::Chesto_Berry,
		Ability::Blaze,
		default_combined_stats<generation>,
		regular_moves(MoveName::Rest)
	);
	pokemon.set_hp(1_bi);
	auto flags = ActivePokemonFlags<generation>();
	auto active_pokemon = AnyMutableActivePokemon(pokemon, flags);

	auto environment = Environment();

	active_pokemon.rest(environment, false);
	CHECK(active_pokemon.status().name() == StatusName::clear);
}

TEST_CASE("Chesto Berry awakens after Rest cures status", "[active_pokemon]") {
	auto pokemon = Pokemon<generation>(
		Species::Charmander,
		Level(100_bi),
		Gender::female,
		Item::Chesto_Berry,
		Ability::Blaze,
		default_combined_stats<generation>,
		regular_moves(MoveName::Rest)
	);
	pokemon.set_hp(1_bi);
	auto flags = ActivePokemonFlags<generation>();
	auto active_pokemon = AnyMutableActivePokemon(pokemon, flags);

	auto environment = Environment();

	active_pokemon.set_status(StatusName::paralysis, environment);
	CHECK(active_pokemon.status().name() == StatusName::paralysis);

	CHECK(clears_status(active_pokemon.item(environment), StatusName::rest));
	active_pokemon.rest(environment, false);

	CHECK(active_pokemon.status().name() == StatusName::clear);
}

} // namespace
} // namespace technicalmachine
