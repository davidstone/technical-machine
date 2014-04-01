// Handles when Bide activates
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

#include "duration.hpp"

#include <cassert>

namespace technicalmachine {
using namespace bounded::literal;

void BideDuration::activate() {
	m_turns_active = 0_bi;
}

BideDuration::operator bool() const {
	return static_cast<bool>(m_turns_active);
}

bool BideDuration::decrement() {
	assert(this->operator bool());
	if (*m_turns_active == max) {
		m_turns_active = bounded::none;
		return true;
	}
	else {
		++*m_turns_active;
		return false;
	}
}

BideDuration::hash_type BideDuration::hash() const {
	return m_turns_active ? static_cast<hash_type>(*m_turns_active + 1_bi) : 0;
}

BideDuration::hash_type BideDuration::max_hash() {
	// Additional 1 for the optional state
	return max + 2;
}

bool operator== (BideDuration const & lhs, BideDuration const & rhs) {
	return lhs.m_turns_active == rhs.m_turns_active;
}

bool operator!= (BideDuration const & lhs, BideDuration const & rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
