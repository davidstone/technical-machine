// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/any_pokemon.hpp>

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template<any_seen_pokemon PokemonType>
auto any_move_matches(PokemonType const pokemon, auto const condition) -> bool {
	return containers::any(pokemon.regular_moves(), [=](Move const move) {
		constexpr auto generation = generation_from<PokemonType>;
		return condition(
			generation,
			KnownMove{move.name(), get_type(generation, move.name(), get_hidden_power_type(pokemon))}
		);
	});
}

auto has_physical_move(any_seen_pokemon auto const pokemon) -> bool {
	return any_move_matches(pokemon, is_physical);
}

auto has_special_move(any_seen_pokemon auto const pokemon) -> bool {
	return any_move_matches(pokemon, is_special);
}

} // namespace technicalmachine
