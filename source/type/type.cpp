// Type function definitions
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

#include "type.hpp"

#include <vector>

#include "effectiveness.hpp"

#include "../rational.hpp"
#include "../status.hpp"
#include "../weather.hpp"

#include "../pokemon/pokemon.hpp"

namespace technicalmachine {

Type::Type (Types name):
	type (name) {
}

bool operator== (Type const lhs, Type const rhs) {
	return lhs.type == rhs.type;
}

bool operator!= (Type const lhs, Type const rhs) {
	return !(lhs == rhs);
}

bool Type::is_boosted_by_flash_fire () const {
	return type == Fire;
}

bool Type::is_immune_to_hail () const {
	return type == Ice;
}

bool Type::is_immune_to_sandstorm () const {
	switch (type) {
		case Ground:
		case Rock:
		case Steel:
			return true;
		default:
			return false;
	}
}

bool Type::is_strengthened_by_weather (Weather const & weather) const {
	return (weather.rain() and type == Water) or (weather.sun() and type == Fire);
}

bool Type::is_weakened_by_weather (Weather const & weather) const {
	return (weather.rain() and type == Fire) or (weather.sun() and type == Water);
}

template<>
bool Type::blocks_status<Status::BURN> () const {
	return type == Fire;
}

template<>
bool Type::blocks_status<Status::FREEZE> () const {
	return type == Ice;
}

template<>
bool Type::blocks_status<Status::POISON> () const {
	switch (type) {
		case Poison:
		case Steel:
			return true;
		default:
			return false;
	}
}

template<>
bool Type::blocks_status<Status::POISON_TOXIC> () const {
	return blocks_status<Status::POISON> ();
}

Effectiveness Type::get_effectiveness(Pokemon const & defender) const {
	return Effectiveness(type, defender);
}

}	// namespace technicalmachine
