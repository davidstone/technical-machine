// Nature data structure
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

#ifndef NATURE_H_
#define NATURE_H_

#include <string>

namespace technicalmachine {

class Nature {
	public:
		enum Natures {
			ADAMANT,
			BASHFUL,
			BOLD,
			BRAVE,
			CALM,
			CAREFUL,
			DOCILE,
			GENTLE,
			HARDY,
			HASTY,
			IMPISH,
			JOLLY,
			LAX,
			LONELY,
			MILD,
			MODEST,
			NAIVE,
			NAUGHTY,
			QUIET,
			QUIRKY,
			RASH,
			RELAXED,
			SASSY,
			SERIOUS,
			TIMID,
			END
		};
		Natures name;
		Nature ();
		bool is_set () const;
		static std::string to_string (Natures name);
		static Natures from_string (std::string const & str);
		void set_name_from_string (std::string const & str);
};

}	// namespace technicalmachine
#endif	// NATURE_H_
