// Nature string conversions
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

#include "nature.hpp"

#include <map>
#include <string>

#include "invalid_string_conversion.hpp"

namespace technicalmachine {

std::string Nature::to_string (Natures name) {
	std::string const nature_converter [] = {
		"Adamant", "Bashful", "Bold", "Brave", "Calm", "Careful",
		"Docile", "Gentle", "Hardy", "Hasty", "Impish", "Jolly",
		"Lax", "Lonely", "Mild", "Modest", "Naive", "Naughty",
		"Quiet", "Quirky", "Rash", "Relaxed", "Sassy", "Serious",
		"Timid", "End_Nature"
	};
	return nature_converter [name];
}

std::string Nature::to_string () const {
	return to_string (name);
}

Nature::Natures Nature::from_string (std::string const & str) {
	static std::map <std::string, Natures> const converter {
		{ "Adamant", ADAMANT },
		{ "Bashful", BASHFUL },
		{ "Bold", BOLD },
		{ "Brave", BRAVE },
		{ "Calm", CALM },
		{ "Careful", CAREFUL },
		{ "Docile", DOCILE },
		{ "Gentle", GENTLE },
		{ "Hardy", HARDY },
		{ "Hasty", HASTY },
		{ "Impish", IMPISH },
		{ "Jolly", JOLLY },
		{ "Lax", LAX },
		{ "Lonely", LONELY },
		{ "Mild", MILD },
		{ "Modest", MODEST },
		{ "Naive", NAIVE },
		{ "Naughty", NAUGHTY },
		{ "Quiet", QUIET },
		{ "Quirky", QUIRKY },
		{ "Rash", RASH },
		{ "Relaxed", RELAXED },
		{ "Sassy", SASSY },
		{ "Serious", SERIOUS },
		{ "Timid", TIMID }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Nature", str);
}

void Nature::set_name_from_string (std::string const & str) {
	name = from_string (str);
}

}	// namespace technicalmachine
