// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.all_moves;

import tm.move.container;

import tm.pokemon.any_pokemon;
import tm.pokemon.max_pokemon_per_team;

import tm.generation;

namespace technicalmachine {

export template<any_active_pokemon PokemonType>
constexpr auto all_moves(PokemonType const pokemon, TeamSize const team_size) {
	return MoveContainer<generation_from<PokemonType>>(pokemon.regular_moves(), team_size);
}

} // namespace technicalmachine
