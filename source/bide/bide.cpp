// Handles bide damage and when it activates
// Copyright (C) 2015 David Stone
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

#include "bide.hpp"

namespace technicalmachine {
using namespace bounded::literal;

auto Bide::is_active() const -> bool {
	return static_cast<bool>(m_duration);
}

auto Bide::activate() -> void {
	m_duration.activate();
}

auto Bide::add_damage(damage_type const damage) -> void {
	if (is_active()) {
		m_damage.add(damage);
	}
}

auto Bide::decrement() -> damage_type {
	return BOUNDED_CONDITIONAL(m_duration.decrement(), m_damage.release(), 0_bi);
}

auto operator== (Bide const lhs, Bide const rhs) -> bool {
	return
		lhs.m_damage == rhs.m_damage and
		lhs.m_duration == rhs.m_duration;
}

}	// namespace technicalmachine
