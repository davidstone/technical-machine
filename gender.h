// Gender header
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GENDER_H_
#define GENDER_H_

#include <string>

namespace technicalmachine {

class Gender {
	public:
		// Silly genderless Pokemon not letting me use a bool.
		enum Genders { GENDERLESS = 0, FEMALE = -1, MALE = 1 };
		Genders gender;
		
		Gender ();
		Gender (Genders gender_);
		Gender & operator= (Gender other);
		Gender & operator= (Genders other);

		// Return 1 for same gender, -1 for opposite, and 0 if either is genderless
		int multiplier (Gender foe) const;
		// Pokemon Lab and Pokemon Online use a value of 2 for female, instead of -1
		void from_simulator_int (int gender_number);
		int to_simulator_int () const;
		void set_name_from_string (std::string const & str);
};

}
#endif		// GENDER_H_
