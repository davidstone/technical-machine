// Random effects of moves
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

#ifndef VARIABLE_HPP_
#define VARIABLE_HPP_

#include <cstdint>

namespace technicalmachine {

// Used for moves with a variable power / length / other integer range. Moves of
// variable power: Magnitude = 4-10, Psywave = 5-15, Present = 0-4 (0 = heal).
// It is also used to determine whether random effects activate.
class Variable {
	public:
		Variable (uint16_t v, uint16_t p);
		uint16_t value () const;
		uint16_t probability () const;
		bool effect_activates () const;
		bool present_heals ()  const;
		unsigned psywave_damage (unsigned level) const;
		void reset_phaze_probabilities (uint8_t size);
		static constexpr uint16_t max_probability = 840;
	private:
		uint16_t m_value;
		uint16_t m_probability;
};
}	// namespace technicalmachine
#endif	// VARIABLE_HPP_
