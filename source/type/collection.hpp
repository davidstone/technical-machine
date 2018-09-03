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

#include "type.hpp"
#include "../pokemon/species_forward.hpp"
#include "../status.hpp"

#include <containers/algorithms/all_any_none.hpp>
#include <containers/array/array.hpp>

#include <type_traits>

namespace technicalmachine {
struct ActivePokemon;
struct MutableActivePokemon;
struct Pokemon;
struct Weather;

namespace detail_type_collection {

struct TypeArray {
private:
	using container_type = containers::array<Type, 2>;
public:
	using const_iterator = container_type::const_iterator;
	constexpr TypeArray(Type const type1):
		m_types({{type1, Type::Typeless}})
		{
	}
	constexpr TypeArray(Type const type1, Type const type2):
		m_types({{type1, type2}})
		{
	}
	friend constexpr auto begin(TypeArray const & array) {
		return begin(array.m_types);
	}
	friend constexpr auto end(TypeArray const & array) {
		auto const size = BOUNDED_CONDITIONAL(array.m_types[1_bi] == Type::Typeless, 1_bi, 2_bi);
		return begin(array) + size;
	}
private:
	container_type m_types;
};

}	// namespace detail_type_collection

struct TypeCollection {
	explicit TypeCollection(Species name);
	friend auto is_immune_to_hail(TypeCollection const collection) -> bool;
	friend auto is_immune_to_sandstorm(TypeCollection const collection) -> bool;
	friend auto blocks_status(TypeCollection const collection, Statuses const status) -> bool {
		return containers::any(collection.types, [=](auto const type) {
			return blocks_status(type, status);
		});
	}
	auto change_type(Type const type) -> void;
private:
	friend auto is_type(Pokemon const & pokemon, Type type, bool roosting) -> bool;
	friend struct Effectiveness;
	detail_type_collection::TypeArray types;
};

auto is_type(Pokemon const & pokemon, Type type, bool roosting) -> bool;

auto grounded(ActivePokemon pokemon, Weather weather) -> bool;
auto grounded(MutableActivePokemon pokemon, Weather weather) -> bool;
auto grounded(Pokemon const & pokemon, Weather weather) -> bool;

}	// namespace technicalmachine
