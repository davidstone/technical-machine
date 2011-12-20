// Convert to / from PO's format
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

#ifndef POKEMON_ONLINE_CONVERSION_H_
#define POKEMON_ONLINE_CONVERSION_H_

#include <string>
#include "../ability.hpp"
#include "../item.hpp"
#include "../stat.hpp"
#include "../move.hpp"
#include "../species.hpp"

namespace technicalmachine {
namespace po {

Species id_to_species (int id);
int species_to_id (Species species);
Ability::Abilities id_to_ability (int id);
int ability_to_id (Ability::Abilities ability);
Item::Items id_to_item (int id);
int item_to_id (Item::Items item);
Move::Moves id_to_move (int id);
int move_to_id (Move::Moves move);
Nature::Natures id_to_nature (int id);
int nature_to_id (Nature::Natures nature);

} // namespace po
} // namespace technicalmachine
#endif // POKEMON_ONLINE_CONVERSION_H_
