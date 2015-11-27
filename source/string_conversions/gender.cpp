// Gender string conversions
// Copyright (C) 2015 David Stone
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

#include "conversion.hpp"
#include "invalid_string_conversion.hpp"

#include "../gender.hpp"

#include <containers/array/make_array.hpp>

#include <map>

namespace technicalmachine {

std::string const & to_string(Gender::Genders const gender) {
	static auto const gender_name = containers::make_array<std::string>(
		"Female", "Genderless", "Male"
	);
	return gender_name[gender];
}

template<>
Gender::Genders from_string(boost::string_ref const str) {
	static std::map<boost::string_ref, Gender::Genders> const converter {
		{ "Genderless", Gender::GENDERLESS },
		{ "None", Gender::GENDERLESS },
		{ "No Gender", Gender::GENDERLESS },
		{ "Female", Gender::FEMALE },
		{ "Male", Gender::MALE }
	};
	auto const it = converter.find(str);
	if (it != converter.end()) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Gender", str);
	}
}

}	// namespace technicalmachine
