// Convert to / from PO's format
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

#pragma once

#include "../../ability.hpp"
#include "../../gender.hpp"
#include "../../item.hpp"

#include "../../move/moves.hpp"

#include "../../pokemon/species_forward.hpp"

#include "../../stat/nature.hpp"
#include "../../stat/stat.hpp"

#include <bounded/integer.hpp>

#include <cstdint>
#include <string>
#include <utility>

namespace technicalmachine {
namespace po {

struct SpeciesIDs {
	using ID = bounded::checked_integer<0, 493>;
	using Forme = bounded::checked_integer<0, 5>;
	ID id;
	Forme forme;	
};
Species id_to_species(SpeciesIDs species);
SpeciesIDs species_to_id(Species species);

using AbilityID = bounded::checked_integer<0, 123>;
Ability::Abilities id_to_ability(AbilityID id);
AbilityID ability_to_id(Ability::Abilities ability);
Ability::Abilities battle_id_to_ability(uint16_t id, uint8_t part);

using GenderID = bounded::checked_integer<0, 2>;
Gender::Genders id_to_gender(GenderID id);
GenderID gender_to_id(Gender::Genders gender);

using ItemID = bounded::checked_integer<0, 226>;
Item id_to_item(ItemID id);
ItemID item_to_id(Item item);
Item battle_id_to_item(uint16_t id, uint8_t part);

using MoveID = bounded::checked_integer<0, (bounded::make(Moves::END) - bounded::make(Moves::Regular_Begin)).value()>;
Moves id_to_move(MoveID id);
MoveID move_to_id(Moves move);

using NatureID = bounded::checked_integer<0, 24>;
Nature id_to_nature(NatureID id);
NatureID nature_to_id(Nature nature);

} // namespace po
} // namespace technicalmachine
