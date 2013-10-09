// Type information for Pokemon
// Copyright (C) 2012 David Stone
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

#include <array>
#include "type.hpp"
#include "../pokemon/species_forward.hpp"
#include "../status.hpp"

namespace technicalmachine {
class ActivePokemon;
class Pokemon;
class Weather;

namespace detail {
class TypeArray {
private:
	typedef std::array<Type, 2> container_type;
public:
	typedef container_type::const_iterator const_iterator;
	explicit TypeArray(Type const type1);
	TypeArray(Type const type1, Type const type2);
	const_iterator begin() const;
	const_iterator end() const;
private:
	typedef container_type::size_type size_type;
	size_type size() const;
	container_type m_types;
};
}

class TypeCollection {
public:
	TypeCollection (Species name);
	bool is_immune_to_hail () const;
	bool is_immune_to_sandstorm () const;
	template<Status::Statuses status>
	bool blocks_status () const {
		for (Type const type : types) {
			if (type.blocks_status<status> ())
				return true;
		}
		return false;
	}
	void change_type(Type const type);
private:
	friend bool is_type(Pokemon const & pokemon, Type type, bool roosting);
	friend class Type;
	friend class Effectiveness;
	detail::TypeArray types;
};

bool is_type(Pokemon const & pokemon, Type type, bool roosting = false);
bool is_type(ActivePokemon const & pokemon, Type type);
bool grounded(Pokemon const & pokemon, Weather const & weather);
bool grounded(ActivePokemon const & pokemon, Weather const & weather);

}	// namespace technicalmachine
#endif	// TYPE__COLLECTION_HPP_
