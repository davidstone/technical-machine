// Moves data structure
// Copyright (C) 2012 David Stone
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

#ifndef MOVE__TIMES_USED_HPP_
#define MOVE__TIMES_USED_HPP_

#include <cstdint>
#include "../rational.hpp"

namespace technicalmachine {

class TimesUsed {
	public:
		TimesUsed();
		void reset ();
		uint64_t hash() const;
		uint64_t max_hash() const;
		void increment();
		bool was_used_last () const;
		unsigned fury_cutter_power() const;
		unsigned momentum_move_power() const;
		unsigned triple_kick_power() const;
		Rational metronome_boost() const;
		friend bool operator== (TimesUsed lhs, TimesUsed rhs);
	private:
		uint8_t counter;
};

bool operator!= (TimesUsed lhs, TimesUsed rhs);

}	// namespace technicalmachine
#endif	// MOVE__TIMES_USED_HPP_
