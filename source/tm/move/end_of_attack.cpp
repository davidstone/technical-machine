// Copyright David Stone 2024.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.move.end_of_attack;

import tm.pokemon.active_pokemon;
import tm.pokemon.any_pokemon;

import tm.environment;
import tm.generation;
import tm.handle_curse;

import bounded;

namespace technicalmachine {
using namespace bounded::literal;

export template<any_mutable_active_pokemon UserPokemon>
constexpr auto end_of_attack(
	UserPokemon const user_pokemon,
	OtherMutableActivePokemon<UserPokemon> const other_pokemon,
	Environment const environment
) -> void {
	if constexpr (generation_from<UserPokemon> <= Generation::two) {
		if (user_pokemon.hp().current() == 0_bi or other_pokemon.hp().current() == 0_bi) {
			return;
		}
		user_pokemon.status_and_leech_seed_effects(other_pokemon, environment);
		handle_curse(user_pokemon, environment);
	}
}

} // namespace technicalmachine
