// Copyright (C) 2016 David Stone
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

#include "../damage.hpp"
#include "../operators.hpp"
#include "../stat/hp.hpp"

#include <bounded/integer.hpp>

namespace technicalmachine {
using namespace bounded::literal;

struct BideDamage {
	auto add(damage_type damage) -> void;
	auto release() -> damage_type;
	friend auto operator== (BideDamage lhs, BideDamage rhs) -> bool;
	
private:
	// This is the greatest range that matters since anything more is overkill
	bounded::clamped_integer<0, (HP::max_value + 1) / 2> m_damage = 0_bi;
};

}	// namespace technicalmachine
