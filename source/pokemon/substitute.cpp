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

#include "substitute.hpp"

namespace technicalmachine {

auto Substitute::create(HP::current_type const total_hp) -> hp_type {
	if (static_cast<bool>(*this)) {
		return 0_bi;
	}
	m_hp = total_hp / 4_bi;
	return m_hp;
}

Substitute::operator bool() const {
	return hp() != 0_bi;
}

bool operator== (Substitute const & lhs, Substitute const & rhs) {
	return lhs.hp() == rhs.hp();
}
bool operator!= (Substitute const & lhs, Substitute const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
