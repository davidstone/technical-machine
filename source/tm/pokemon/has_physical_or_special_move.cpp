// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.has_physical_or_special_move;

import tm.pokemon.any_pokemon;

import tm.move.category;
import tm.move.known_move;
import tm.move.move;

import tm.pokemon.get_hidden_power_type;
import tm.pokemon.pokemon;

import tm.type.type;

import tm.generation;

import containers;

namespace technicalmachine {

template<any_seen_pokemon PokemonType>
constexpr auto any_move_matches(PokemonType const pokemon, auto const condition) -> bool {
	return containers::any(pokemon.regular_moves(), [=](Move const move) {
		constexpr auto generation = generation_from<PokemonType>;
		return condition(
			generation,
			KnownMove{move.name(), get_type(generation, move.name(), get_hidden_power_type(pokemon))}
		);
	});
}

export constexpr auto has_physical_move(any_seen_pokemon auto const pokemon) -> bool {
	return any_move_matches(pokemon, is_physical);
}

export constexpr auto has_special_move(any_seen_pokemon auto const pokemon) -> bool {
	return any_move_matches(pokemon, is_special);
}

} // namespace technicalmachine
