// Handles bide damage
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

#include "damage.hpp"

#include <algorithm>

namespace technicalmachine {

BideDamage::BideDamage():
	damage(0)
	{
}

namespace {
constexpr BideDamage::hash_type max_hp = 714;
constexpr BideDamage::hash_type max_bide_damage = (max_hp + 1) / 2;

}	// unnamed namespace

void BideDamage::add(unsigned const extra_damage) {
	damage += extra_damage;
	damage = std::min(static_cast<hash_type>(damage), max_bide_damage);
}

unsigned BideDamage::release() {
	unsigned const temp = damage;
	damage = 0;
	return temp;
}

BideDamage::hash_type BideDamage::hash() const {
	return damage;
}

BideDamage::hash_type BideDamage::max_hash() {
	return max_bide_damage + 1;
}

bool operator== (BideDamage const & lhs, BideDamage const & rhs) {
	return lhs.damage == rhs.damage;
}

bool operator!= (BideDamage const & lhs, BideDamage const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
