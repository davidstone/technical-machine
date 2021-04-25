// Convert to / from PO's format
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/ability.hpp>
#include <tm/gender.hpp>
#include <tm/item.hpp>

#include <tm/move/moves.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <tm/stat/nature.hpp>

#include <bounded/integer.hpp>

#include <numeric_traits/min_max_value.hpp>

#include <cstdint>
#include <string>
#include <utility>

namespace technicalmachine {
namespace po {

struct SpeciesIDs {
	using ID = bounded::integer<1, 493>;
	using Forme = bounded::integer<0, 5>;
	ID id;
	Forme forme;	
};
Species id_to_species(SpeciesIDs species);
SpeciesIDs species_to_id(Species species);

using AbilityID = bounded::integer<1, 123>;
Ability id_to_ability(AbilityID id);
AbilityID ability_to_id(Ability ability);
Ability battle_id_to_ability(uint16_t id, uint8_t part);

using GenderID = bounded::integer<0, 2>;
Gender id_to_gender(GenderID id);
GenderID gender_to_id(Gender gender);

using ItemID = bounded::integer<0, 226>;
Item id_to_item(ItemID id);
ItemID item_to_id(Item item);
Item battle_id_to_item(uint16_t id, uint8_t part);

using MoveID = bounded::integer<1, static_cast<int>(bounded::constant<numeric_traits::max_value<Moves>> - bounded::integer(Moves::Regular_Begin) + 1_bi)>;
Moves id_to_move(MoveID id);
MoveID move_to_id(Moves move);

using NatureID = bounded::integer<0, 24>;
Nature id_to_nature(NatureID id);
NatureID nature_to_id(Nature nature);

} // namespace po
} // namespace technicalmachine
