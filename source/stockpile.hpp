// Class that handles Stockpile
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

#ifndef STOCKPILE_HPP_
#define STOCKPILE_HPP_

#include <cstdint>

namespace technicalmachine {
class Rational;

class Stockpile {
	public:
		Stockpile();
		// Returns whether Stockpile was able to increment (true) or if it is
		// already maxed out (false)
		bool increment();
		// Returns the amount of Stockpile lost. This allows correct subtraction
		// for stat boosts.
		int release();
		void reset();
		unsigned spit_up_power() const;
		static Rational swallow_healing(int const stockpiles);
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
		friend bool operator== (Stockpile const & lhs, Stockpile const & rhs);
	private:
		friend class Score;
		uint8_t level;
};
bool operator!= (Stockpile const & lhs, Stockpile const & rhs);

}	// namespace technicalmachine
#endif	// STOCKPILE_HPP_
