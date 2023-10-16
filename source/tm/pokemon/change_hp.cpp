// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.change_hp;

import tm.pokemon.any_pokemon;

import tm.environment;

import bounded;

namespace technicalmachine {

export constexpr auto change_hp(any_mutable_active_pokemon auto const pokemon, Environment const environment, bounded::bounded_integer auto const change) -> void {
	pokemon.set_hp(environment, pokemon.hp().current() + change);
}

} // namespace technicalmachine
