// Chance to hit calculations
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

#ifndef STAT__CHANCE_TO_HIT_HPP_
#define STAT__CHANCE_TO_HIT_HPP_

#include <cstdint>

namespace technicalmachine {
class ActivePokemon;
class Rational;
class Weather;

class ChanceToHit {
public:
	ChanceToHit();
	Rational operator()() const;
	void update(ActivePokemon const & user, ActivePokemon const & target, Weather const & weather, bool target_moved);
private:
	uint8_t probability;
};

}	// namespace technicalmachine
#endif	// STAT__CHANCE_TO_HIT_HPP_
