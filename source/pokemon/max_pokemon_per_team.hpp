// The maximum number of Pokemon possible on a team
// Copyright (C) 2014 David Stone
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

#ifndef POKEMON_MAX_POKEMON_PER_TEAM_HPP_
#define POKEMON_MAX_POKEMON_PER_TEAM_HPP_

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {
using namespace bounded_integer::literal;

constexpr auto max_pokemon_per_team = 6_bi;

}	// namespace technicalmachine
#endif	// POKEMON_MAX_POKEMON_PER_TEAM_HPP_
