// Chance to hit calculations
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

#ifndef STAT__CHANCE_TO_HIT_HPP_
#define STAT__CHANCE_TO_HIT_HPP_

#include <cstdint>

namespace technicalmachine {
class Team;
class Weather;

class ChanceToHit {
	public:
		typedef uint8_t value_type;
		ChanceToHit();
		value_type operator()() const;
		value_type inverse() const;
		bool can_miss() const;
		void update(Team const & user, Team const & target, Weather const & weather);
	private:
		value_type probability;
};

}	// namespace technicalmachine
#endif	// STAT__CHANCE_TO_HIT_HPP_
