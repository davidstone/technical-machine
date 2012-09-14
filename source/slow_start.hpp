// Class that handles Slow Start's timer
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

#ifndef SLOW_START_HPP_
#define SLOW_START_HPP_

#include <cstdint>

namespace technicalmachine {

class SlowStart {
	public:
		SlowStart();
		bool is_active() const;
		void decrement();
		void reset();
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
		friend bool operator== (SlowStart const & lhs, SlowStart const & rhs);
	private:
		friend class Score;
		uint8_t turns_remaining;
};
bool operator!= (SlowStart const & lhs, SlowStart const & rhs);

}	// namespace technicalmachine
#endif	// SLOW_START_HPP_
