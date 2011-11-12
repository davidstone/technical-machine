// Convert to / from PL's format
// Copyright (C) 2011 David Stone
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

#ifndef POKEMON_LAB_CONVERSION_H_
#define POKEMON_LAB_CONVERSION_H_

#include "../move.h"
#include "../species.h"

namespace technicalmachine {
namespace pl {

Species id_to_species (int id);
int species_to_id (Species species);
constexpr Move::Moves id_to_move (int id);
constexpr int move_to_id (Move::Moves move);

// Pokemon Lab uses the same format as Technical Machine for any conversions not listed here.

} // namespace pl
} // namespace technicalmachine
#endif // POKEMON_LAB_CONVERSION_H_
