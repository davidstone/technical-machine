// Class to represent Uproar's counter
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

#include "uproar.hpp"

// Uproar lasts 2-5 turns

namespace technicalmachine {
using namespace bounded::literal;

auto Uproar::increment() -> void {
	if (!m_counter) {
		m_counter = 1_bi;
	}
	else if (*m_counter != std::numeric_limits<decltype(m_counter)>::max()) {
		++*m_counter;
	}
	else {
		m_counter = bounded::none;
	}
}

auto Uproar::hash() const -> hash_type {
	return m_counter ? static_cast<hash_type>(*m_counter) : 0;
}

auto Uproar::max_hash() -> hash_type {
	return static_cast<hash_type>(std::numeric_limits<decltype(m_counter)::value_type>::max() + 1_bi);
}

auto operator== (Uproar const & lhs, Uproar const & rhs) -> bool {
	return lhs.m_counter == rhs.m_counter;
}

auto operator!= (Uproar const & lhs, Uproar const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
