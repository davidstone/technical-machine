// Class that handles Doom Desire / Future Sight
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

#ifndef DELAYED_ATTACK_HPP_
#define DELAYED_ATTACK_HPP_

#include <cstdint>

namespace technicalmachine {

class DelayedAttack {
	public:
		DelayedAttack();
		void activate();
		// Returns whether Doom Desire / Future Sight hits this turn
		bool decrement();
		void reset();
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
		friend bool operator== (DelayedAttack const & lhs, DelayedAttack const & rhs);
	private:
		friend class Score;
		bool is_active() const;
		uint8_t turns_before_hitting;
};
bool operator!= (DelayedAttack const & lhs, DelayedAttack const & rhs);

}	// namespace technicalmachine
#endif	// DELAYED_ATTACK_HPP_
