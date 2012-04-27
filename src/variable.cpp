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

#include "variable.hpp"

namespace technicalmachine {

Variable::Variable (uint16_t v, uint16_t p):
	m_value (v),
	m_probability (p) {
}

uint16_t Variable::value () const {
	return m_value;
}

uint16_t Variable::probability () const {
	return m_probability;
}

bool Variable::effect_activates () const {
	return value() != 0;
}

bool Variable::present_heals () const {
	return value() != 0;
}

unsigned Variable::psywave_damage (unsigned level) const {
	return level * value() / 10;
}

void Variable::reset_phaze_probabilities (uint8_t size) {
	m_probability = (size > 2) ? max_probability / (size - 1) : max_probability;
}

}	// namespace technicalmachine
