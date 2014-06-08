// Class that handles Outrage, Petal Dance, and Thrash
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

#include "rampage.hpp"

namespace technicalmachine {

auto Rampage::is_active() const -> bool {
	return static_cast<bool>(m_turns_active);
}

auto Rampage::activate() -> void {
	if (!is_active()) {
		m_turns_active = 0_bi;
	}
}

auto Rampage::advance_one_turn() -> bool {
	if (!is_active()) {
		return false;
	}
	if (*m_turns_active == std::numeric_limits<type>::max()) {
		m_turns_active = bounded::none;
		return true;
	}
	++*m_turns_active;
	return false;
}

auto operator==(Rampage const lhs, Rampage const rhs) -> bool {
	return lhs.m_turns_active == rhs.m_turns_active;
}

auto operator!= (Rampage const lhs, Rampage const rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
