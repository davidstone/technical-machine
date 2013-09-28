// Nature string conversions
// Copyright (C) 2013 David Stone
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

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

#include "../stat/nature.hpp"

namespace technicalmachine {

std::string to_string(Nature::Natures const name) {
	std::string const nature_converter [] = {
		"Adamant", "Bashful", "Bold", "Brave", "Calm", "Careful",
		"Docile", "Gentle", "Hardy", "Hasty", "Impish", "Jolly",
		"Lax", "Lonely", "Mild", "Modest", "Naive", "Naughty",
		"Quiet", "Quirky", "Rash", "Relaxed", "Sassy", "Serious",
		"Timid", "End_Nature"
	};
	return nature_converter [name];
}

template<>
Nature::Natures from_string<Nature::Natures>(std::string const & str) {
	static std::map <std::string, Nature::Natures> const converter {
		{ "Adamant", Nature::ADAMANT },
		{ "Bashful", Nature::BASHFUL },
		{ "Bold", Nature::BOLD },
		{ "Brave", Nature::BRAVE },
		{ "Calm", Nature::CALM },
		{ "Careful", Nature::CAREFUL },
		{ "Docile", Nature::DOCILE },
		{ "Gentle", Nature::GENTLE },
		{ "Hardy", Nature::HARDY },
		{ "Hasty", Nature::HASTY },
		{ "Impish", Nature::IMPISH },
		{ "Jolly", Nature::JOLLY },
		{ "Lax", Nature::LAX },
		{ "Lonely", Nature::LONELY },
		{ "Mild", Nature::MILD },
		{ "Modest", Nature::MODEST },
		{ "Naive", Nature::NAIVE },
		{ "Naughty", Nature::NAUGHTY },
		{ "Quiet", Nature::QUIET },
		{ "Quirky", Nature::QUIRKY },
		{ "Rash", Nature::RASH },
		{ "Relaxed", Nature::RELAXED },
		{ "Sassy", Nature::SASSY },
		{ "Serious", Nature::SERIOUS },
		{ "Timid", Nature::TIMID }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Nature", str);
}

}	// namespace technicalmachine
