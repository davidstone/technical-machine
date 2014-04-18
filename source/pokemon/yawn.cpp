// Class to represent Yawn's counter
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

#include "yawn.hpp"

namespace technicalmachine {
using namespace bounded::literal;
constexpr auto used_this_turn = 0_bi;
constexpr auto causes_sleep_this_turn = 1_bi;

auto Yawn::activate() -> void {
	if (!m_counter) {
		m_counter = used_this_turn;
	}
}

auto Yawn::advance_turn() -> bool {
	if (!m_counter) {
		return false;
	}
	else if (*m_counter == used_this_turn) {
		*m_counter = causes_sleep_this_turn;
		return false;
	}
	else {
		m_counter = bounded::none;
		return true;
	}
}

// It is decremented at the end of the turn, and hashes are calculated after the
// end of turn.
auto Yawn::hash() const -> hash_type {
	return !m_counter ? 0 : static_cast<hash_type>(*m_counter);
}

auto Yawn::max_hash() -> hash_type {
	return static_cast<hash_type>(std::numeric_limits<decltype(m_counter)::value_type>::max() + 1_bi);
}

auto operator==(Yawn const & lhs, Yawn const & rhs) -> bool {
	return lhs.m_counter == rhs.m_counter;
}

auto operator!=(Yawn const & lhs, Yawn const & rhs) -> bool {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
