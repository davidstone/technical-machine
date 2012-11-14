// Declaration of functions that block selection / execution
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

#ifndef BLOCK_HPP_
#define BLOCK_HPP_

#include <cstddef>
#include <vector>
#include "pokemon/species_forward.hpp"

namespace technicalmachine {

class ActivePokemon;
class Move;
class Weather;

class LegalSelections {
		typedef std::vector<Move const *> Container;
	public:
		typedef Container::const_iterator const_iterator;
		LegalSelections(ActivePokemon const & user, ActivePokemon const & other, Weather const & weather);
		Species species() const;
		const_iterator begin() const;
		const_iterator end() const;
		size_t size() const;
		Move const * const & operator[] (size_t index) const;
	private:
		Container container;
		Species m_species;
};

bool can_execute_move (ActivePokemon & user, ActivePokemon const & other, Weather const & weather);

}	// namespace technicalmachine
#endif	// BLOCK_HPP_
