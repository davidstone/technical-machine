// Pokemon Lab Species ID
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

#ifndef POKEMON_LAB__SPECIES_ID_HPP_
#define POKEMON_LAB__SPECIES_ID_HPP_

#include <cstdint>
#include "../../gender.hpp"
#include "../../move/moves_forward.hpp"
#include "../../pokemon/species_forward.hpp"

namespace technicalmachine {
namespace pl {

template<typename T>
class IDType;
template<>
class IDType<Species> {
public:
	typedef uint16_t type;
};
template<>
class IDType<Moves> {
public:
	typedef uint16_t type;
};
template<>
class IDType<Gender::Genders> {
public:
	typedef uint8_t type;
};

template<typename T>
class ID {
public:
	typedef typename IDType<T>::type underlying_type;
	constexpr explicit ID(underlying_type const id):
		m_value(id) {
	}
	constexpr underlying_type value() const {
		return m_value;
	}
private:
	underlying_type m_value;
};



} // namespace pl
} // namespace technicalmachine
#endif // POKEMON_LAB__SPECIES_ID_HPP_
