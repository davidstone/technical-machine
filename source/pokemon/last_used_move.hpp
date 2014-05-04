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

#ifndef POKEMON__LAST_USED_MOVE_HPP_
#define POKEMON__LAST_USED_MOVE_HPP_

#include <bounded_integer/bounded_integer.hpp>
#include <bounded_integer/optional.hpp>

namespace technicalmachine {
template<typename Numerator, typename Denominator>
class bounded_rational;

class LastUsedMove {
public:
	LastUsedMove();
	using index_type = bounded::checked_integer<0, 3>;
	bool has_moved() const;
	bool was_used_last(index_type index_of_move) const;
	void reset();
	void increment(index_type index_of_move);
	bounded::integer<10, 160> fury_cutter_power() const;
	bounded::integer<30, 480> momentum_move_power() const;
	bounded::integer<0, 30> triple_kick_power() const;
	bounded_rational<bounded::integer<10, 20>, bounded::integer<10, 10>> metronome_boost() const;
	uint64_t hash() const;
	uint64_t max_hash() const;
	friend bool operator== (LastUsedMove lhs, LastUsedMove rhs);
private:
	bounded::optional<index_type> m_index_of_move;
	bounded::clamped_integer<0, 10> m_consecutive_turns_used;
};

bool operator!= (LastUsedMove lhs, LastUsedMove rhs);

}	// namespace technicalmachine
#endif	// POKEMON__LAST_USED_MOVE_HPP_
