// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/pokemon/pokemon.hpp>

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>

#include <tm/generation.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {

auto any_move_matches(Generation const generation, RegularMoves const moves, bounded::optional<Type> const hidden_power_type, auto const condition) -> bool {
	return containers::any(moves, [=](Move const move) {
		return condition(
			generation,
			KnownMove{move.name(), get_type(generation, move.name(), hidden_power_type)}
		);
	});
}

inline auto has_physical_move(Generation const generation, RegularMoves const moves, bounded::optional<Type> const hidden_power_type) -> bool {
	return any_move_matches(generation, moves, hidden_power_type, [](auto... args) { return is_physical(args...); });
}

template<Generation generation>
auto has_physical_move(Pokemon<generation> const pokemon) -> bool {
	return any_move_matches(
		generation,
		pokemon.regular_moves(),
		get_hidden_power_type(pokemon),
		[](auto... args) { return is_physical(args...); }
	);
}

template<Generation generation>
auto has_special_move(Pokemon<generation> const pokemon) -> bool {
	return any_move_matches(
		generation,
		pokemon.regular_moves(),
		get_hidden_power_type(pokemon),
		[](auto... args) { return is_special(args...); }
	);
}

} // namespace technicalmachine
