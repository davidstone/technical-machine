// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.is_type;

import tm.pokemon.any_pokemon;

import tm.type.type;

import containers;

namespace technicalmachine {

export constexpr auto is_type(any_active_pokemon auto const pokemon, auto const... types) -> bool requires(sizeof...(types) > 0) {
	return (... or (
		(types != Type::Flying or !pokemon.last_used_move().is_roosting()) and
		containers::any_equal(pokemon.types(), types)
	));
}

} // namespace technicalmachine
