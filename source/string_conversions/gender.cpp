// Gender string conversions
// Copyright (C) 2018 David Stone
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

#include <map>

namespace technicalmachine {

std::string_view to_string(Gender const gender) {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::male: return "Male";
		case Gender::genderless: return "Genderless";
	}
}

template<>
Gender from_string(std::string_view const str) {
	static std::map<std::string_view, Gender> const converter {
		{ "Genderless", Gender::genderless },
		{ "None", Gender::genderless },
		{ "No Gender", Gender::genderless },
		{ "Female", Gender::female },
		{ "Male", Gender::male }
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Gender", str);
	}
}

}	// namespace technicalmachine
