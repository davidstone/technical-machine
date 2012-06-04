// Convert to / from PL's format
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

#ifndef POKEMON_LAB__CONVERSION_HPP_
#define POKEMON_LAB__CONVERSION_HPP_

#include "../gender.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
enum class Species : uint16_t;
namespace pl {

Gender::Genders id_to_gender (unsigned id);
unsigned gender_to_id (Gender::Genders gender);
Moves id_to_move (unsigned id);
unsigned move_to_id (Moves move);
Species id_to_species (unsigned id);
unsigned species_to_id (Species species);

// Pokemon Lab uses the same format as Technical Machine for any conversions not listed here.

}	// namespace pl
}	// namespace technicalmachine
#endif	// POKEMON_LAB__CONVERSION_HPP_
