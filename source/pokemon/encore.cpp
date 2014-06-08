// Encore class
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

#include "encore.hpp"

namespace technicalmachine {

bool Encore::is_active() const {
	return static_cast<bool>(m_turns_active);
}

void Encore::activate() {
	m_turns_active = 0_bi;
}

void Encore::advance_one_turn() {
	if (!is_active()) {
		return;
	}
	if (*m_turns_active == std::numeric_limits<type>::max()) {
		m_turns_active = bounded::none;
	} else {
		++*m_turns_active;
	}
}

bool operator== (Encore const & lhs, Encore const & rhs) {
	return lhs.m_turns_active == rhs.m_turns_active;
}

bool operator!= (Encore const & lhs, Encore const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
