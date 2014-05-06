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

#ifndef BIDE__DAMAGE_HPP_
#define BIDE__DAMAGE_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include "../damage.hpp"
#include "../stat/hp.hpp"

namespace technicalmachine {

class BideDamage {
public:
	BideDamage();
	auto add(damage_type damage) -> void;
	auto release() -> damage_type;
	friend auto operator== (BideDamage lhs, BideDamage rhs) -> bool;
	typedef uint64_t hash_type;
	auto hash() const -> hash_type;
	static auto max_hash() -> hash_type;
private:
	// This is the greatest range that matters since anything more is overkill
	bounded::clamped_integer<0, (HP::max_value + 1) / 2> m_damage;
};

auto operator!= (BideDamage lhs, BideDamage rhs) -> bool;

}	// namespace technicalmachine
#endif	// BIDE__DAMAGE_HPP_
