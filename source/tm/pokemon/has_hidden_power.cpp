// Copyright David Stone 2023.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.pokemon.has_hidden_power;

import tm.move.move;
import tm.move.move_name;

import tm.pokemon.any_pokemon;

import containers;

namespace technicalmachine {

export auto has_hidden_power(any_pokemon auto const & pokemon) -> bool {
	return containers::maybe_find_if(pokemon.regular_moves(), [](Move const move) {
		return move.name() == MoveName::Hidden_Power;
	});
}

} // namespace technicalmachine
