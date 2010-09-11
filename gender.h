// Gender enumeration
// Copyright 2010 David Stone
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

#include <map>

// Silly genderless Pokemon not letting me use a bool.

enum genders { GENDERLESS = 0, FEMALE = -1, MALE = 1 };

void set_gender_map (std::map <std::string, genders> &genders_map) {
	genders_map["None"] = GENDERLESS;
	genders_map["Female"] = FEMALE;
	genders_map["Male"] = MALE;
}


#endif
