// Gender string conversions
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

#include "gender.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

namespace technicalmachine {

std::string Gender::to_string (Genders gender) {
	static std::string const gender_name [] {
		"Female", "Genderless", "Male"
	};
	return gender_name [gender];
}

std::string Gender::to_string () const {
	return to_string (gender);
}

Gender::Genders Gender::from_string (std::string const & str) {
	static std::map <std::string, Genders> const converter {
		{ "Genderless", GENDERLESS },
		{ "None", GENDERLESS },
		{ "No Gender", GENDERLESS },
		{ "Female", FEMALE },
		{ "Male", MALE }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Gender", str);
}

Gender::Gender (std::string const & str):
	gender (from_string (str))
	{
}

}	// namespace technicalmachine
