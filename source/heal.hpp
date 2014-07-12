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

#ifndef HEAL_HPP_
#define HEAL_HPP_

#include "ability.hpp"
#include "rational.hpp"

#include "pokemon/active_pokemon.hpp"

#include <bounded_integer/bounded_integer.hpp>

namespace technicalmachine {

template<typename Numerator, typename Denominator>
void heal(ActivePokemon & pokemon, bounded_rational<Numerator, Denominator> const rational) {
	if (is_fainted(pokemon)) {
		return;
	}
	auto const hp_healed = get_hp(pokemon).max() * rational;
	bool const is_positive = rational > make_rational(0_bi, 1_bi);
	if (is_positive) {
		get_hp(pokemon) += bounded::max(hp_healed, 1_bi);
	} else if (!get_ability(pokemon).blocks_secondary_damage()) {
		get_hp(pokemon) += bounded::min(hp_healed, -1_bi);
	}
}

}	// namespace technicalmachine
#endif	// HEAL_HPP_
