// Exception class if phazing attempts to bring in the currently active Pokemon
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

#ifndef PHAZING_IN_SAME_POKEMON_HPP_
#define PHAZING_IN_SAME_POKEMON_HPP_

#include <stdexcept>
#include <string>

namespace technicalmachine {

class PhazingInSamePokemon : public std::logic_error {
	public:
		PhazingInSamePokemon (unsigned index):
			std::logic_error ("Attempted to phaze in the currently active Pokemon at index " + std::to_string(index) + ".")
			{
		}
};

}	// namespace technicalmachine
#endif	// PHAZING_IN_SAME_POKEMON_HPP_
