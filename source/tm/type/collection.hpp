// Type information for Pokemon
// Copyright (C) 2016 David Stone
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

#include <tm/type/type.hpp>
#include <tm/pokemon/species_forward.hpp>
#include <tm/status.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/array/array.hpp>

namespace technicalmachine {
struct ActivePokemon;
enum class Generation : std::uint8_t;
struct MutableActivePokemon;
struct Pokemon;
struct Weather;

struct TypeCollection {
	explicit TypeCollection(Generation, Species name);
	friend auto is_immune_to_hail(TypeCollection const collection) -> bool;
	friend auto is_immune_to_sandstorm(TypeCollection const collection) -> bool;
	friend auto blocks_status(TypeCollection const collection, Statuses const status) -> bool {
		return containers::any(collection.types, [=](auto const type) {
			return blocks_status(type, status);
		});
	}
	auto change_type(Type const type) -> void;
private:
	friend auto is_type(Pokemon const &, Type, bool roosting) -> bool;
	friend struct Effectiveness;
	containers::array<Type, 2> types;
};

auto is_type(Pokemon const &, Type, bool roosting) -> bool;

auto grounded(ActivePokemon pokemon, Weather weather) -> bool;
auto grounded(MutableActivePokemon pokemon, Weather weather) -> bool;
auto grounded(Pokemon const & pokemon, Weather weather) -> bool;

}	// namespace technicalmachine
