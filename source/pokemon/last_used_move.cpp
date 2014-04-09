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
namespace {
using namespace bounded::literal;
constexpr uint64_t max_number_of_moves = 4;
}	// namespace

LastUsedMove::LastUsedMove() :
	m_consecutive_turns_used(0_bi) {
}

bool LastUsedMove::has_moved() const {
	return static_cast<bool>(m_index_of_move);
}

bool LastUsedMove::was_used_last(index_type const index_of_move) const {
	return has_moved() and *m_index_of_move == index_of_move;
}

void LastUsedMove::increment(index_type const index_of_move, bounded::checked_integer<1, 4> const number_of_regular_moves) {
	assert(index_of_move <= number_of_regular_moves);
	m_index_of_move = index_of_move;
	++m_consecutive_turns_used;
}

void LastUsedMove::reset () {
	*this = LastUsedMove{};
}

bounded::integer<10, 160> LastUsedMove::fury_cutter_power() const {
	// 10 * 2 ^ n
	return 10_bi << bounded::min(m_consecutive_turns_used, 4_bi);
}

bounded::integer<30, 480> LastUsedMove::momentum_move_power() const {
	return 30_bi << bounded::min(m_consecutive_turns_used, 4_bi);
}

bounded::integer<0, 30> LastUsedMove::triple_kick_power() const {
	return 10_bi * bounded::min(m_consecutive_turns_used, 3_bi);
}

bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>> LastUsedMove::metronome_boost() const {
	return make_rational(10_bi + m_consecutive_turns_used, 10_bi);
}

uint64_t LastUsedMove::hash() const {
	auto const index = static_cast<bool>(m_index_of_move) ? static_cast<uint64_t>(*m_index_of_move) : max_number_of_moves;
	return index + (max_number_of_moves + 1) * static_cast<uint64_t>(m_consecutive_turns_used);
}

uint64_t LastUsedMove::max_hash() const {
	return (10 + 1) * (max_number_of_moves + 1);
}

bool operator== (LastUsedMove const lhs, LastUsedMove const rhs) {
	return
		lhs.m_index_of_move == rhs.m_index_of_move and
		lhs.m_consecutive_turns_used == rhs.m_consecutive_turns_used;
}
bool operator!= (LastUsedMove const lhs, LastUsedMove const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
