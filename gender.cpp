// Gender functions
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <map>
#include <string>
#include "gender.h"

namespace technicalmachine {

Gender::Gender ():
	gender (MALE)		// No sexism here!
	{
}

Gender::Gender (Genders gender_):
	gender (gender_) {
}

int Gender::multiplier (Gender foe) const {
	// Return 1 for same gender, -1 for opposite, and 0 if either is genderless
	return gender * foe.gender;
}

void Gender::from_simulator_int (int gender_number) {
	switch (gender_number) {
		case 0:
			gender = GENDERLESS;
			break;
		case 1:
			gender = MALE;
			break;
		case 2:
			gender = FEMALE;
			break;
	}
}

int Gender::to_simulator_int () const {
	int output;
	switch (gender) {
		case GENDERLESS:
			output = 0;
			break;
		case MALE:
			output = 1;
			break;
		case FEMALE:
			output = 2;
			break;
	}
	return output;
}

Gender & Gender::operator= (Gender other) {
	gender = other.gender;
	return *this;
}

Gender & Gender::operator= (Genders other) {
	gender = other;
	return *this;
}

class Map {
	public:
		std::map <std::string, Gender::Genders> gender;
		Map () {
			gender ["None"] = Gender::GENDERLESS;
			gender ["No Gender"] = Gender::GENDERLESS;
			gender ["Female"] = Gender::FEMALE;
			gender ["Male"] = Gender::MALE;
		}
		Gender::Genders find (std::string const & str) const {
			return gender.find (str)->second;
		}
};

void Gender::set_name_from_string (std::string const & str) {
	static Map const map;
	gender = map.find (str);
}
}
