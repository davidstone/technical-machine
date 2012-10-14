// Function to change a Pokemon's HP by a fractional multiplier
// Copyright (C) 2012 David Stone
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

#include <algorithm>

#include "ability.hpp"
#include "damage.hpp"
#include "rational.hpp"

#include "pokemon/pokemon.hpp"

namespace technicalmachine {

void heal(Pokemon & member, Rational const & rational, bool positive) {
	if (member.is_fainted())
		return;
	unsigned const hp_healed = member.hp().max * rational;
	if (positive) {
		member.apply_healing(std::max(hp_healed, 1u));
	}
	else if (!member.ability().blocks_secondary_damage()) {
		member.apply_damage(std::max(hp_healed, 1u));
	}
}

void drain(Pokemon & member, Rational const & rational) {
	heal(member, rational, false);
}

}	// namespace technicalmachine
