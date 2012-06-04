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
class Pokemon;
class Weather;

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
		Type (Types name);
		friend bool operator== (Type lhs, Type rhs);
		friend bool operator!= (Type lhs, Type rhs);
		bool is_boosted_by_flash_fire () const;
		bool is_immune_to_hail () const;
		bool is_immune_to_sandstorm () const;
		bool is_strengthened_by_weather (Weather const & weather) const;
		bool is_weakened_by_weather (Weather const & weather) const;
		template<Status::Statuses status>
		bool blocks_status () const {
			return false;
		}
		// 0, 1, 2, 4, 8, or 16
		unsigned get_effectiveness (Pokemon const & defender) const;
		std::vector <unsigned> get_effectiveness_variables (Pokemon const & pokemon) const;
		static unsigned stealth_rock_effectiveness (Pokemon const & pokemon);
	private:
		// Calculate the effectiveness of attacking type on defending type. 0 is
		// no effect, 1 is not very effective, 2 is neutral, and 4 is super
		// effective.
		static unsigned lookup_effectiveness (Types attacking, Type defending);
		unsigned lookup_effectiveness (Type defending) const;
		static unsigned get_effectiveness (Type::Types type, Pokemon const & defender);
		Types type;
};

}	// namespace technicalmachine
#endif	// TYPE_HPP_
