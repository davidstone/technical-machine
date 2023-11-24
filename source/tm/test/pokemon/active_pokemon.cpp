// Copyright David Stone 2022.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <bounded/assert.hpp>

export module tm.test.pokemon.active_pokemon;

import tm.move.move;
import tm.move.move_name;

import tm.pokemon.active_pokemon;
import tm.pokemon.level;
import tm.pokemon.pokemon;
import tm.pokemon.species;

import tm.stat.default_evs;

import tm.status.clears_status;
import tm.status.status_name;

import tm.test.pokemon_init;

import tm.ability;
import tm.environment;
import tm.gender;
import tm.generation;
import tm.item;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

constexpr auto generation = Generation::four;

// Chesto Berry awakens from Rest
static_assert([]{
	auto pokemon = make_pokemon<generation>({
		.species = Species::Charmander,
		.item = Item::Chesto_Berry,
		.ability = Ability::Blaze,
		.moves = {{
			MoveName::Rest,
		}}
	});
	pokemon.set_hp(1_bi);
	auto flags = ActivePokemonFlags<generation>();
	auto active_pokemon = AnyMutableActivePokemon(pokemon, flags);

	constexpr auto environment = Environment();

	active_pokemon.rest(environment, false);
	return active_pokemon.status().name() == StatusName::clear;
}());

// Chesto Berry awakens after Rest cures status
static_assert([]{
	auto pokemon = make_pokemon<generation>({
		.species = Species::Charmander,
		.item = Item::Chesto_Berry,
		.ability = Ability::Blaze,
		.moves = {{
			MoveName::Rest,
		}}
	});
	pokemon.set_hp(1_bi);
	auto flags = ActivePokemonFlags<generation>();
	auto active_pokemon = AnyMutableActivePokemon(pokemon, flags);

	constexpr auto environment = Environment();

	active_pokemon.set_status(StatusName::paralysis, environment);
	BOUNDED_ASSERT(active_pokemon.status().name() == StatusName::paralysis);

	BOUNDED_ASSERT(clears_status(active_pokemon.item(environment), StatusName::rest));
	active_pokemon.rest(environment, false);

	return active_pokemon.status().name() == StatusName::clear;
}());

} // namespace technicalmachine
