// Exception if unable to find a Pokemon that should exist
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

#ifndef POKEMON__POKEMON_NOT_FOUND_HPP_
#define POKEMON__POKEMON_NOT_FOUND_HPP_

#include <stdexcept>
#include "../string_conversions/conversion.hpp"

namespace technicalmachine {
enum class Species : uint16_t;

class PokemonNotFound : public std::logic_error {
	public:
		PokemonNotFound(Species const species):
			std::logic_error("Unable to find " + to_string(species) + ".") {
		}
};
}	// namespace technicalmachine
#endif	// POKEMON__POKEMON_NOT_FOUND_HPP_
