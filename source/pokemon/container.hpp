// Basic container for Pokemon that helps with bounds checking
// Copyright (C) 2015 David Stone
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

#ifndef POKEMON__CONTAINER_HPP_
#define POKEMON__CONTAINER_HPP_

#include "max_pokemon_per_team.hpp"
#include "pokemon.hpp"

#include "../fixed_capacity_vector.hpp"

namespace technicalmachine {

using PokemonContainer = fixed_capacity_vector<Pokemon, max_pokemon_per_team.value()>;

}	// namespace technicalmachine
#endif	// POKEMON__CONTAINER_HPP_
