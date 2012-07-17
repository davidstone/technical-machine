// Screens
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

#ifndef SCREEN_HPP_
#define SCREEN_HPP_

#include <cstdint>

namespace technicalmachine {

// Lucky Chant, Mist, and Safeguard use the base class
class Screen {
	public:
		Screen();
		void activate();
		void decrement();
		explicit operator bool() const;
		uint64_t hash() const;
		static uint64_t max_hash();
		friend bool operator==(Screen const & lhs, Screen const & rhs);
	protected:
		void set_duration(uint8_t duration);
	private:
		uint8_t turns_remaining;
		static uint8_t max_length();
		friend class Score;
};
bool operator!=(Screen const & lhs, Screen const & rhs);

class ReflectLightScreen : public Screen {
	public:
		void activate(bool is_extended);
		void clear();
		static uint64_t max_hash();
	private:
		static uint8_t max_length();
};

class Tailwind : public Screen {
	public:
		void activate();
		static uint64_t max_hash();
	private:
		static uint8_t max_length();
};

}	// namespace technicalmachine
#endif	// SCREEN_HPP_
