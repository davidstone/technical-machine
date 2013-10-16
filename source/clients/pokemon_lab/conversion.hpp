// Convert to / from PL's format
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

#ifndef POKEMON_LAB__CONVERSION_HPP_
#define POKEMON_LAB__CONVERSION_HPP_

#include <string>
#include "id.hpp"
#include "../../gender.hpp"
#include "../../move/moves_forward.hpp"
#include "../../pokemon/species_forward.hpp"

namespace technicalmachine {
namespace pl {

ID<Gender::Genders> simulator_cast(Gender::Genders gender);
ID<Moves> simulator_cast(Moves move);
ID<Species> simulator_cast(Species species);
std::string to_simulator_string(Species const species);

Gender::Genders simulator_cast(ID<Gender::Genders> id);
Moves simulator_cast(ID<Moves> id);
Species simulator_cast(ID<Species> id);
Species from_simulator_string(std::string const & str);

}	// namespace pl
}	// namespace technicalmachine
#endif	// POKEMON_LAB__CONVERSION_HPP_
