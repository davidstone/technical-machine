// Nature string conversions
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

#include "nature.hpp"

#include "invalid_string_conversion.hpp"

#include "../stat/nature.hpp"

#include <map>

namespace technicalmachine {

std::string const & to_string(Nature const nature) {
	static auto const nature_converter = bounded::make_array<std::string>(
		"Adamant", "Bashful", "Bold", "Brave", "Calm", "Careful",
		"Docile", "Gentle", "Hardy", "Hasty", "Impish", "Jolly",
		"Lax", "Lonely", "Mild", "Modest", "Naive", "Naughty",
		"Quiet", "Quirky", "Rash", "Relaxed", "Sassy", "Serious",
		"Timid", "End_Nature"
	);
	return nature_converter.at(nature);
}

template<>
Nature from_string<Nature>(boost::string_ref const str) {
	static std::map<boost::string_ref, Nature> const converter {
		{ "Adamant", Nature::Adamant },
		{ "Bashful", Nature::Bashful },
		{ "Bold", Nature::Bold },
		{ "Brave", Nature::Brave },
		{ "Calm", Nature::Calm },
		{ "Careful", Nature::Careful },
		{ "Docile", Nature::Docile },
		{ "Gentle", Nature::Gentle },
		{ "Hardy", Nature::Hardy },
		{ "Hasty", Nature::Hasty },
		{ "Impish", Nature::Impish },
		{ "Jolly", Nature::Jolly },
		{ "Lax", Nature::Lax },
		{ "Lonely", Nature::Lonely },
		{ "Mild", Nature::Mild },
		{ "Modest", Nature::Modest },
		{ "Naive", Nature::Naive },
		{ "Naughty", Nature::Naughty },
		{ "Quiet", Nature::Quiet },
		{ "Quirky", Nature::Quirky },
		{ "Rash", Nature::Rash },
		{ "Relaxed", Nature::Relaxed },
		{ "Sassy", Nature::Sassy },
		{ "Serious", Nature::Serious },
		{ "Timid", Nature::Timid }
	};
	auto const it = converter.find(str);
	if (it != converter.end()) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Nature", str);
	}
}

}	// namespace technicalmachine
