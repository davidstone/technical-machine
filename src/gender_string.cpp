// Gender string conversions
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

#include "gender.hpp"

#include <map>
#include <string>

namespace technicalmachine {

void Gender::set_name_from_string (std::string const & str) {
	static std::map <std::string, Genders> const converter {
		{ "None", GENDERLESS },
		{ "No Gender", GENDERLESS },
		{ "Female", FEMALE },
		{ "Male", MALE }
	};
	gender = converter.find (str)->second;
}

}	// namespace technicalmachine
