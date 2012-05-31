// All Screens a team can set up
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

#ifndef SCREENS_HPP_
#define SCREENS_HPP_

#include <cstdint>
#include "screen.hpp"

namespace technicalmachine {

class Screens {
	public:
		void decrement();

		bool light_screen() const;
		bool reflect() const;
		bool lucky_chant() const;
		bool mist() const;
		bool safeguard() const;
		bool tailwind() const;

		void activate_light_screen(bool is_extended = false);
		void activate_reflect(bool is_extended = false);
		void activate_lucky_chant();
		void activate_mist();
		void activate_safeguard();
		void activate_tailwind();

		void shatter();
		typedef uint64_t hash_type;
		uint64_t hash() const;
		static uint64_t max_hash();
		friend bool operator==(Screens const & lhs, Screens const & rhs);
	private:
		ReflectLightScreen m_light_screen;
		ReflectLightScreen m_reflect;
		Screen m_lucky_chant;
		Screen m_mist;
		Screen m_safeguard;
		Tailwind m_tailwind;
		friend class Score;
};
bool operator!=(Screens const & lhs, Screens const & rhs);

}	// namespace technicalmachine
#endif	// SCREENS_HPP_
