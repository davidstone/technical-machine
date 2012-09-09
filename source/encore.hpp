// Encore class
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

#ifndef ENCORE_HPP_
#define ENCORE_HPP_

#include <cstdint>

namespace technicalmachine {

class Encore {
	public:
		Encore();
		bool is_active() const;
		void activate();
		void increment();
		void reset();
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
		friend bool operator== (Encore const & lhs, Encore const & rhs);
	private:
		friend class Score;
		uint8_t turns_active;
};
bool operator!= (Encore const & lhs, Encore const & rhs);

}	// namespace technicalmachine
#endif	// ENCORE_HPP_
