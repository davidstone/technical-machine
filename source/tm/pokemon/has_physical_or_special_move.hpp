// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/pokemon.hpp>

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>

#include <containers/algorithms/all_any_none.hpp>

namespace technicalmachine {

template<Generation generation>
auto any_move_matches(Pokemon<generation> const pokemon, auto const condition) -> bool {
	return containers::any(pokemon.regular_moves(), [=](Move const move) {
		return condition(
			generation,
			KnownMove{move.name(), get_type(generation, move.name(), get_hidden_power_type(pokemon))}
		);
	});
}

template<Generation generation>
auto has_physical_move(Pokemon<generation> const pokemon) -> bool {
	return any_move_matches(pokemon, is_physical);
}

template<Generation generation>
auto has_special_move(Pokemon<generation> const pokemon) -> bool {
	return any_move_matches(pokemon, is_special);
}

} // namespace technicalmachine
