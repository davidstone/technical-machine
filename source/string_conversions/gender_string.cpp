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

#include "gender_string.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

namespace technicalmachine {

std::string to_string(Gender::Genders const gender) {
	static std::string const gender_name [] {
		"Female", "Genderless", "Male"
	};
	return gender_name [gender];
}

template<>
Gender::Genders from_string(std::string const & str) {
	static std::map <std::string, Gender::Genders> const converter {
		{ "Genderless", Gender::GENDERLESS },
		{ "None", Gender::GENDERLESS },
		{ "No Gender", Gender::GENDERLESS },
		{ "Female", Gender::FEMALE },
		{ "Male", Gender::MALE }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Gender", str);
}

}	// namespace technicalmachine
