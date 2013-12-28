// Handles bide damage and when it activates
// Copyright (C) 2013 David Stone
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
using namespace bounded_integer::literal;

bool Bide::is_active() const {
	return static_cast<bool>(m_duration);
}

void Bide::activate() {
	m_duration.activate();
}

void Bide::add_damage(damage_type const damage) {
	if (is_active()) {
		m_damage.add(damage);
	}
}

void Bide::reset() {
	m_damage = BideDamage{};
	m_duration = BideDuration{};
}

damage_type Bide::decrement() {
	return BOUNDED_INTEGER_CONDITIONAL(m_duration.decrement(), m_damage.release(), 0_bi);
}

Bide::hash_type Bide::hash() const {
	return m_duration.hash() + m_duration.max_hash() *
			m_damage.hash();
}

Bide::hash_type Bide::max_hash() {
	return BideDuration::max_hash() * BideDamage::max_hash();
}

bool operator== (Bide const lhs, Bide const rhs) {
	return
		lhs.m_damage == rhs.m_damage and
		lhs.m_duration == rhs.m_duration;
}

bool operator!= (Bide const lhs, Bide const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
