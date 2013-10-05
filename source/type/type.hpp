// Type information
// Copyright (C) 2013 David Stone
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

#ifndef TYPE__TYPE_HPP_
#define TYPE__TYPE_HPP_

#include "../status.hpp"
#include "../move/moves_forward.hpp"

namespace technicalmachine {
class Effectiveness;
class Pokemon;
class Weather;

class Type {
public:
	enum Types : uint8_t {
		Bug,
		Dark,
		Dragon,
		Electric,
		Fighting,
		Fire,
		Flying,
		Ghost,
		Grass,
		Ground,
		Ice,
		Normal,
		Poison,
		Psychic,
		Rock,
		Steel,
		Water,
		Typeless
	};
	Type (Types name);
	Type(Moves move, Pokemon const & pokemon);
	friend bool operator== (Type lhs, Type rhs);
	friend bool operator!= (Type lhs, Type rhs);
	bool is_boosted_by_flash_fire () const;
	bool is_immune_to_hail () const;
	bool is_immune_to_sandstorm () const;
	bool is_strengthened_by_weather (Weather const & weather) const;
	bool is_weakened_by_weather (Weather const & weather) const;
	// Template specializations are defined in cpp
	template<Status::Statuses status>
	bool blocks_status () const {
		return false;
	}
	Effectiveness get_effectiveness(Pokemon const & defender) const;
private:
	friend class Effectiveness;
	Types type;
};

}	// namespace technicalmachine
#endif	// TYPE__TYPE_HPP_
