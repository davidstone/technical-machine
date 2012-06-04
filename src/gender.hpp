// Gender header
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

#ifndef GENDER_HPP_
#define GENDER_HPP_

#include <cstdint>
#include <string>

namespace technicalmachine {

class Gender {
	public:
		enum Genders : uint8_t { FEMALE, GENDERLESS, MALE, END };
		
		Gender ();
		explicit Gender (Genders gender_);
		explicit Gender (std::string const & str);
		Gender & operator= (Gender other);
		Gender & operator= (Genders other);

		// Return 1 if the same, -1 if opposite, and 0 if either is genderless
		int multiplier (Gender foe) const;
		static std::string to_string (Genders gender);
		std::string to_string () const;
		static Genders from_string (std::string const & str);
		friend bool operator== (Gender lhs, Gender rhs);

		Genders gender;
};
bool operator!= (Gender lhs, Gender rhs);

}	// namespace technicalmachine
#endif	// GENDER_HPP_
