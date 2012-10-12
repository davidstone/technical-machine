// Nature data structure
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

#ifndef STAT__NATURE_HPP_
#define STAT__NATURE_HPP_

#include <cstdint>
#include <string>

#include "stat.hpp"

namespace technicalmachine {
class Rational;

class Nature {
	public:
		enum Natures : uint8_t {
			ADAMANT, BASHFUL, BOLD, BRAVE, CALM,
			CAREFUL, DOCILE, GENTLE, HARDY, HASTY,
			IMPISH, JOLLY, LAX, LONELY, MILD, MODEST,
			NAIVE, NAUGHTY, QUIET, QUIRKY, RASH,
			RELAXED, SASSY, SERIOUS, TIMID, END
		};
		Nature ();
		Nature (std::string const & str);
		bool is_set () const;
		void set_if_unknown (Natures nature);
		template<Stat::Stats>
		Rational boost() const;
		std::string to_string () const;
		friend bool operator== (Nature lhs, Nature rhs);

		Natures name;
};
bool operator!= (Nature lhs, Nature rhs);

}	// namespace technicalmachine
#endif	// STAT__NATURE_HPP_
