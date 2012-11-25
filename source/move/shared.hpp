// Handle common moves that all Pokemon can select
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

#ifndef MOVE__SHARED_HPP_
#define MOVE__SHARED_HPP_

#include <cstddef>
#include <cstdint>

namespace technicalmachine {
class Move;

class SharedMoves {
	public:
		explicit SharedMoves (unsigned team_size = 6);
		void remove_switch();
		Move const & operator[](size_t index) const;
		size_t size() const;
		template<typename Function>
		void for_each(Function && f) const {
			for (size_t n = 0; n != size(); ++n) {
				f(operator[](n));
			}
		}
		friend bool operator==(SharedMoves const & lhs, SharedMoves const & rhs);
	private:
		uint8_t number_of_switches;
};

}	// namespace technicalmachine
#endif	// MOVE__SHARED_HPP_
