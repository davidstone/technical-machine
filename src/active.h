// Allows safe access to the active Pokemon / move
// Copyright (C) 2011 David Stone
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

#ifndef ACTIVE_H_
#define ACTIVE_H_

#include <cstddef>
#include <vector>

namespace technicalmachine {

template <class T>
class Active {
	public:
		std::vector<T> set;			// All Pokemon on the team, all moves on the Pokemon, etc.
		size_t index;
		Active () :
			index (0) {
		}
		T * operator-> () {
			return &set [index];
		}
		T const * operator->() const {
			return &set [index];
		}
		T & operator*() {
			return set [index];
		}
		T const & operator*() const {
			return set [index];
		}
};

}
#endif		// ACTIVE_H_
