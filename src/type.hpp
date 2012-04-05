// Type information
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

#ifndef TYPE_HPP_
#define TYPE_HPP_

#include <vector>
#include "status.hpp"

namespace technicalmachine {

class Type {
	public:
		enum Types : uint8_t {
			BUG,
			DARK,
			DRAGON,
			ELECTRIC,
			FIGHTING,
			FIRE,
			FLYING,
			GHOST,
			GRASS,
			GROUND,
			ICE,
			NORMAL,
			POISON,
			PSYCHIC,
			ROCK,
			STEEL,
			WATER,
			TYPELESS
		};
		Types type;
		
		Type (Types name);
		bool operator== (Type other) const;
		bool operator!= (Type other) const;
		bool is_immune_to_sandstorm () const;
		template<Status::Statuses status>
		bool blocks_status () const {
			return false;
		}
};

class TypeCollection {
	public:
		std::vector <Type> types;
		
		bool is_immune_to_sandstorm () const;
		template<Status::Statuses status>
		bool blocks_status () const {
			for (Type const type : types) {
				if (type.blocks_status<status> ())
					return true;
			}
			return false;
		}
};

class Pokemon;
class Team;
class Weather;

bool is_type (Team const & team, Type type);
unsigned get_effectiveness (Type type, Pokemon const & pokemon);
std::vector <unsigned> get_effectiveness_variables (Type type, Pokemon const & pokemon);
bool grounded (Team const & team, Weather const & weather);

}	// namespace technicalmachine
#endif	// TYPE_HPP_
