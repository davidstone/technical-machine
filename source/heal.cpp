// Function to change a Pokemon's HP by a fractional multiplier
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

#include "heal.hpp"

#include <bounded_integer/bounded_integer.hpp>

#include "ability.hpp"
#include "rational.hpp"

#include "pokemon/active_pokemon.hpp"
#include "pokemon/pokemon.hpp"

namespace technicalmachine {

void heal(ActivePokemon & pokemon, Rational const & rational, bool positive) {
	if (pokemon.is_fainted())
		return;
	unsigned const hp_healed = static_cast<unsigned>(get_hp(pokemon).max()) * rational;
	if (positive) {
		get_hp(pokemon) += bounded_integer::max(hp_healed, 1_bi);
	}
	else if (!get_ability(pokemon).blocks_secondary_damage()) {
		get_hp(pokemon) -= bounded_integer::max(hp_healed, 1_bi);
	}
}

void drain(ActivePokemon & pokemon, Rational const & rational) {
	heal(pokemon, rational, false);
}

}	// namespace technicalmachine
