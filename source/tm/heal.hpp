// Function to change a Pokemon's HP by a fractional multiplier
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

#include <tm/ability.hpp>
#include <tm/rational.hpp>

#include <tm/pokemon/active_pokemon.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {

template<typename Numerator, typename Denominator>
void heal(MutableActivePokemon pokemon, rational<Numerator, Denominator> const scale) {
	auto const hp = get_hp(pokemon);
	if (hp == 0_bi) {
		return;
	}
	auto const hp_healed = hp.max() * scale;
	if (scale > 0_bi) {
		change_hp(pokemon, bounded::max(hp_healed, 1_bi));
	} else if (!blocks_secondary_damage(get_ability(pokemon))) {
		change_hp(pokemon, bounded::min(hp_healed, -1_bi));
	}
}

}	// namespace technicalmachine
