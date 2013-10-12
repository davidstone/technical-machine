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

#ifndef HEAL_HPP_
#define HEAL_HPP_

namespace technicalmachine {

class ActivePokemon;
class Pokemon;
class Rational;

void apply_healing(Pokemon & pokemon, unsigned amount);
void heal(ActivePokemon & pokemon, Rational const & rational, bool positive = true);
void drain(ActivePokemon & pokemon, Rational const & rational);

}	// namespace technicalmachine
#endif	// HEAL_HPP_
