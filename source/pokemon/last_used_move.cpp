// Keeps track of the last used move
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

#include "last_used_move.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>

#include "../rational.hpp"

namespace technicalmachine {
namespace {
constexpr uint8_t max_number_of_moves = 4;
constexpr uint8_t max_relevant_counter_value = 10;
}	// namespace

LastUsedMove::LastUsedMove() :
	m_index_of_move(max_number_of_moves),
	m_consecutive_turns_used(0) {
}

bool LastUsedMove::has_moved() const {
	return m_index_of_move != max_number_of_moves;
}

bool LastUsedMove::was_used_last(uint8_t const index_of_move) const {
	assert(index_of_move < max_number_of_moves);
	return m_index_of_move == index_of_move;
}

void LastUsedMove::increment(uint8_t const index_of_move, uint8_t const number_of_regular_moves) {
	if (index_of_move >= number_of_regular_moves) {
		return;
	}
	m_index_of_move = index_of_move;
	if (m_consecutive_turns_used < max_relevant_counter_value) {
		++m_consecutive_turns_used;
	}
}

void LastUsedMove::reset () {
	*this = LastUsedMove{};
}

unsigned LastUsedMove::fury_cutter_power() const {
	// 10 * 2 ^ n
	return 10u << std::min(static_cast<unsigned>(m_consecutive_turns_used), 4u);
}

unsigned LastUsedMove::momentum_move_power() const {
	return 30u << std::min(static_cast<unsigned>(m_consecutive_turns_used), 4u);
}

unsigned LastUsedMove::triple_kick_power() const {
	return 10 * std::min(static_cast<unsigned>(m_consecutive_turns_used), 3u);
}

Rational LastUsedMove::metronome_boost() const {
	assert(m_consecutive_turns_used <= 10);
	return Rational(10u + m_consecutive_turns_used, 10);
}

uint64_t LastUsedMove::hash() const {
	return m_index_of_move + (max_number_of_moves + 1) * static_cast<uint64_t>(m_consecutive_turns_used);
}

uint64_t LastUsedMove::max_hash() const {
	return (max_relevant_counter_value + 1) * (max_number_of_moves + 1);
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
