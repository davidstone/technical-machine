// PP class
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

#ifndef MOVE__PP_HPP_
#define MOVE__PP_HPP_

#include <cstdint>

namespace technicalmachine {
enum class Moves : uint16_t;
class Ability;

class Pp {
	public:
		Pp(Moves move, unsigned pp_ups);
		uint64_t hash () const;
		uint64_t max_hash () const;
		bool is_empty() const;
		bool has_unlimited_pp() const;
		void decrement(Ability const & foe_ability);
		unsigned trump_card_power() const;
		friend bool operator== (Pp const & lhs, Pp const & rhs);
	private:
		uint8_t max;
		uint8_t current;
};
bool operator!= (Pp const & lhs, Pp const & rhs);

}	// namespace technicalmachine
#endif	// MOVE__PP_HPP_
