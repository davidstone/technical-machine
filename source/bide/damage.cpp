// Handles bide damage
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

#include "damage.hpp"

#include <algorithm>

namespace technicalmachine {
using namespace bounded::literal;

auto BideDamage::add(damage_type const damage) -> void {
	m_damage += damage;
}

auto BideDamage::release() -> damage_type {
	bounded::clamped_integer<0, HP::max_value> const output_damage = m_damage * 2_bi;
	m_damage = 0_bi;
	return output_damage;
}

auto operator== (BideDamage const lhs, BideDamage const rhs) -> bool {
	return lhs.m_damage == rhs.m_damage;
}

}	// namespace technicalmachine
