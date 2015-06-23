// Convert to / from PO's format
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

#pragma once

#include <cstdint>
#include <string>
#include <utility>

#include "../../ability.hpp"
#include "../../gender.hpp"
#include "../../item.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species_forward.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

namespace technicalmachine {
namespace po {

Species id_to_species (unsigned id, unsigned forme);
std::pair <uint16_t, uint8_t> species_to_id (Species species);
Ability::Abilities id_to_ability (unsigned id);
unsigned ability_to_id (Ability::Abilities ability);
Ability::Abilities battle_id_to_ability (uint16_t id, uint8_t part);
Gender::Genders id_to_gender (unsigned id);
unsigned gender_to_id (Gender::Genders gender);
Item id_to_item(unsigned id);
unsigned item_to_id(Item item);
Item battle_id_to_item(uint16_t id, uint8_t part);
Moves id_to_move (unsigned id);
unsigned move_to_id (Moves move);
Nature id_to_nature(unsigned id);
unsigned nature_to_id(Nature nature);

} // namespace po
} // namespace technicalmachine
