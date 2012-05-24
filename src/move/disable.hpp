// Disable class
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

#ifndef MOVE__DISABLE_HPP_
#define MOVE__DISABLE_HPP_

#include <cstdint>

namespace technicalmachine {

class Disable {
	public:
		Disable();
		void activate();
		void advance_one_turn();
		void reset();
		explicit operator bool() const;
		uint64_t hash() const;
		static uint64_t max_hash();
		friend bool operator== (Disable lhs, Disable rhs);
	private:
		// Number of turns this move has been Disabled (4-7)
		uint8_t counter;
};

bool operator!= (Disable lhs, Disable rhs);

}	// namespace technicalmachine
#endif	// MOVE__DISABLE_HPP_
