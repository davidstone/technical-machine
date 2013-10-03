// Which Move is disabled and for how long
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

#include "disable.hpp"

#include <cassert>
#include <cstdint>
#include <limits>

namespace technicalmachine {
namespace {

constexpr uint8_t max_number_of_moves = 4;
// Struggle and switching cannot be disabled
constexpr auto inactive = max_number_of_moves;
constexpr uint8_t max_disable_length = 7;

}	// unnamed namespace

Disable::Disable() :
	m_index_of_disabled_move(inactive),
	m_turns_disabled(0) {
}

void Disable::activate(uint8_t const index_of_disabled_move) {
	assert(index_of_disabled_move < inactive);
	m_index_of_disabled_move = index_of_disabled_move;
}

void Disable::advance_one_turn() {
	if (m_index_of_disabled_move == inactive) {
		return;
	}
	// TODO: update with proper probability actions
	if (m_turns_disabled < max_disable_length)
		++m_turns_disabled;
	else
		reset();
}

void Disable::reset() {
	m_index_of_disabled_move = inactive;
	m_turns_disabled = 0;
}

bool Disable::move_is_disabled(uint8_t const index_of_move_to_check) const {
	assert(index_of_move_to_check < inactive);
	return m_index_of_disabled_move == index_of_move_to_check;
}

uint64_t Disable::hash() const {
	return static_cast<uint64_t>(m_turns_disabled) * max_disable_length + m_index_of_disabled_move;
}

uint64_t Disable::max_hash() {
	return max_disable_length * (max_number_of_moves + 1);
}

bool operator== (Disable const lhs, Disable const rhs) {
	return
		lhs.m_turns_disabled == rhs.m_turns_disabled and
		lhs.m_index_of_disabled_move == rhs.m_index_of_disabled_move;
}

bool operator!= (Disable const lhs, Disable const rhs) {
	return !(lhs == rhs);
}

}	// namespace technicalmachine
