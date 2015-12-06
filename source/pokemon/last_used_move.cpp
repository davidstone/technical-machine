// Keeps track of the last used move
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

#include "last_used_move.hpp"

#include <cassert>

#include "../rational.hpp"

namespace technicalmachine {
using namespace bounded::literal;

auto LastUsedMove::has_moved() const -> bool {
	return static_cast<bool>(m_index_of_move);
}

auto LastUsedMove::was_used_last(index_type const index_of_move) const -> bool {
	return has_moved() and *m_index_of_move == index_of_move;
}

auto LastUsedMove::increment(index_type const index_of_move) -> void {
	m_index_of_move = index_of_move;
	++m_consecutive_turns_used;
}

auto LastUsedMove::fury_cutter_power() const -> bounded::integer<10, 160> {
	// 10 * 2 ^ n
	return 10_bi << bounded::min(m_consecutive_turns_used, 4_bi);
}

auto LastUsedMove::momentum_move_power() const -> bounded::integer<30, 480> {
	return 30_bi << bounded::min(m_consecutive_turns_used, 4_bi);
}

auto LastUsedMove::triple_kick_power() const -> bounded::integer<0, 30> {
	return 10_bi * bounded::min(m_consecutive_turns_used, 3_bi);
}

auto LastUsedMove::metronome_boost() const -> bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>> {
	return make_rational(10_bi + m_consecutive_turns_used, 10_bi);
}

auto operator== (LastUsedMove const lhs, LastUsedMove const rhs) -> bool {
	return
		lhs.m_index_of_move == rhs.m_index_of_move and
		lhs.m_consecutive_turns_used == rhs.m_consecutive_turns_used;
}

}	// namespace technicalmachine
