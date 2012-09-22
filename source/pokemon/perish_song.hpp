// Perish Song duration
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

#ifndef PERISH_SONG_HPP_
#define PERISH_SONG_HPP_

#include <cstdint>

namespace technicalmachine {

class PerishSong {
	public:
		PerishSong();
		void activate();
		// return value: whether this Pokemon faints
		bool next_turn();
		void reset();
		friend bool operator== (PerishSong const & lhs, PerishSong const & rhs);
		typedef uint64_t hash_type;
		hash_type hash() const;
		static hash_type max_hash();
	private:
		bool is_active() const;
		uint8_t duration;
};

bool operator!= (PerishSong const & lhs, PerishSong const & rhs);

}	// namespace technicalmachine
#endif	// PERISH_SONG_HPP_
