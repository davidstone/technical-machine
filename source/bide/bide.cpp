// Handles bide damage and when it activates
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

#include "bide.hpp"

namespace technicalmachine {

bool Bide::is_active() const {
	return static_cast<bool>(duration);
}

void Bide::activate() {
	duration.activate();
}

void Bide::add_damage(unsigned const extra_damage) {
	if (is_active())
		damage.add(extra_damage);
}

void Bide::reset() {
	damage = BideDamage();
	duration = BideDuration();
}

unsigned Bide::decrement() {
	return duration.decrement() ? damage.release() : 0;
}

Bide::hash_type Bide::hash() const {
	return duration.hash() + duration.max_hash() *
			damage.hash();
}

Bide::hash_type Bide::max_hash() {
	return BideDuration::max_hash() * BideDamage::max_hash();
}

bool operator== (Bide const & lhs, Bide const & rhs) {
	return lhs.damage == rhs.damage and
			lhs.duration == rhs.duration;
}

bool operator!= (Bide const & lhs, Bide const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
