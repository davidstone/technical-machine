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

#ifndef POKEMON__LAST_USED_MOVE_HPP_
#define POKEMON__LAST_USED_MOVE_HPP_

#include <cstdint>

namespace technicalmachine {
class Rational;

class LastUsedMove {
public:
	LastUsedMove();
	bool has_moved() const;
	bool was_used_last(uint8_t index_of_move) const;
	void reset();
	void increment(uint8_t index_of_move, uint8_t number_of_regular_moves);
	unsigned fury_cutter_power() const;
	unsigned momentum_move_power() const;
	unsigned triple_kick_power() const;
	Rational metronome_boost() const;
	uint64_t hash() const;
	uint64_t max_hash() const;
	friend bool operator== (LastUsedMove lhs, LastUsedMove rhs);
private:
	uint8_t m_index_of_move;
	uint8_t m_consecutive_turns_used;
};

bool operator!= (LastUsedMove lhs, LastUsedMove rhs);

}	// namespace technicalmachine
#endif	// POKEMON__LAST_USED_MOVE_HPP_
