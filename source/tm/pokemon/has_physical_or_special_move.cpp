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

#include <tm/pokemon/has_physical_or_special_move.hpp>

#include <tm/move/category.hpp>
#include <tm/move/known_move.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/algorithms/transform.hpp>

namespace technicalmachine {
namespace {

auto any_move_matches(Generation const generation, containers::static_vector<Moves, max_moves_per_pokemon.value()> const moves, bounded::optional<Type> const hidden_power_type, auto const condition) -> bool {
	return containers::any(moves, [=](Moves const move) {
		return condition(
			generation,
			KnownMove{move, get_type(generation, move, hidden_power_type)}
		);
	});
}

auto get_move_names(Pokemon const pokemon) {
	return containers::static_vector<Moves, max_moves_per_pokemon.value()>(
		containers::transform(pokemon.regular_moves(), [](Move const move) {
			return move.name();
		})
	);
}

} // namespace

auto has_physical_move(Generation const generation, containers::static_vector<Moves, max_moves_per_pokemon.value()> const moves, bounded::optional<Type> const hidden_power_type) -> bool {
	return any_move_matches(generation, moves, hidden_power_type, is_physical);
}

auto has_physical_move(Generation const generation, Pokemon const pokemon) -> bool {
	return any_move_matches(
		generation,
		get_move_names(pokemon),
		get_hidden_power_type(pokemon),
		is_physical
	);
}

auto has_special_move(Generation const generation, Pokemon const pokemon) -> bool {
	return any_move_matches(
		generation,
		get_move_names(pokemon),
		get_hidden_power_type(pokemon),
		is_special
	);
}

} // namespace technicalmachine
