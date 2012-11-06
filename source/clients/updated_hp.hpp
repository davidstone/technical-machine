// Keep track of changes in HP to make sure TM has the same view as the server
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

#ifndef CLIENTS__UPDATED_HP_HPP_
#define CLIENTS__UPDATED_HP_HPP_

#include <map>
#include "../pokemon/species_forward.hpp"

namespace technicalmachine {
class Pokemon;
class Team;

class UpdatedHP {
	public:
		explicit UpdatedHP(Team const & team);
		void reset_between_turns();
		void add(bool is_me, Pokemon const & pokemon, unsigned max_precision);
		void update(bool is_me, Pokemon const & pokemon, unsigned value);
		void direct_damage(bool is_me, Pokemon const & pokemon, unsigned damage);
		unsigned get(bool is_me, Pokemon const & pokemon) const;
		unsigned damage(bool is_me, Pokemon const & pokemon) const;
	private:
		typedef std::pair<bool, Species> key_type;
		typedef std::pair<unsigned, unsigned> mapped_type;
		std::map<key_type, mapped_type> container;
};

}	// namespace technicalmachine
#endif	// CLIENTS__UPDATED_HP_HPP_
