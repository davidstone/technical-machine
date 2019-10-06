// Copyright (C) 2018 David Stone
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

#pragma once

#include <tm/stat/hp.hpp>
#include <tm/operators.hpp>

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct BideDamage {
	constexpr auto add(HP::current_type const damage) -> void {
		m_damage += damage;
	}

	constexpr auto release() {
		bounded::clamped_integer<0, HP::max_value> const output_damage = m_damage * 2_bi;
		m_damage = 0_bi;
		return output_damage;
	}

	friend constexpr auto operator==(BideDamage const lhs, BideDamage const rhs) {
		return lhs.m_damage == rhs.m_damage;
	}
	
private:
	// This is the greatest range that matters since anything more is overkill
	bounded::clamped_integer<0, (HP::max_value + 1) / 2> m_damage = 0_bi;
};

}	// namespace technicalmachine
