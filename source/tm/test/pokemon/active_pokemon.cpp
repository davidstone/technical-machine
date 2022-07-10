// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/pokemon/active_pokemon.hpp>

#include <catch2/catch_test_macros.hpp>

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

	auto weather = Weather();

	active_pokemon.rest(weather, false);
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

	auto weather = Weather();

	active_pokemon.set_status(StatusName::paralysis, weather);
	CHECK(active_pokemon.status().name() == StatusName::paralysis);

	CHECK(clears_status(active_pokemon.item(weather), StatusName::rest));
	active_pokemon.rest(weather, false);

	CHECK(active_pokemon.status().name() == StatusName::clear);
}

} // namespace
} // namespace technicalmachine
