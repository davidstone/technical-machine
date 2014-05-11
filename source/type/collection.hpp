// Type information for Pokemon
// Copyright (C) 2014 David Stone
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

#ifndef TYPE__COLLECTION_HPP_
#define TYPE__COLLECTION_HPP_

#include "type.hpp"
#include "../pokemon/species_forward.hpp"
#include "../status.hpp"

#include <bounded_integer/array.hpp>

namespace technicalmachine {
class ActivePokemon;
class Pokemon;
class Weather;

namespace detail_type_collection {

class TypeArray {
private:
	using container_type = bounded::array<Type, 2>;
public:
	using const_iterator = container_type::const_iterator;
	explicit constexpr TypeArray(Type const type1):
		m_types({{type1, Type::Typeless}})
		{
	}
	constexpr TypeArray(Type const type1, Type const type2):
		m_types({{type1, type2}})
		{
	}
	auto begin() const -> const_iterator;
	auto end() const -> const_iterator;
private:
	container_type m_types;
};

}	// namespace detail_type_collection

class TypeCollection {
public:
	TypeCollection (Species name);
	friend auto is_immune_to_hail(TypeCollection const collection) -> bool;
	friend auto is_immune_to_sandstorm(TypeCollection const collection) -> bool;
	template<Status::Statuses status>
	friend auto blocks_status(TypeCollection const collection) -> bool {
		for (auto const type : collection.types) {
			if (blocks_status<status>(type)) {
				return true;
			}
		}
		return false;
	}
	auto change_type(Type const type) -> void;
private:
	friend auto is_type(Pokemon const & pokemon, Type type, bool roosting) -> bool;
	friend class Effectiveness;
	detail_type_collection::TypeArray types;
};

auto is_type(Pokemon const & pokemon, Type type, bool roosting = false) -> bool;
auto is_type(ActivePokemon const & pokemon, Type type) -> bool;
auto grounded(Pokemon const & pokemon, Weather const & weather) -> bool;
auto grounded(ActivePokemon const & pokemon, Weather const & weather) -> bool;

}	// namespace technicalmachine
#endif	// TYPE__COLLECTION_HPP_
