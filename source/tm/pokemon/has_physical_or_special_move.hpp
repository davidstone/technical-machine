// Copyright (C) 2020 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

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
