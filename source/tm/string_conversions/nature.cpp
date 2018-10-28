// Nature string conversions
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

#include <tm/string_conversions/nature.hpp>

#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <tm/stat/nature.hpp>

#include <map>

namespace technicalmachine {

std::string_view to_string(Nature const nature) {
	switch (nature) {
		case Nature::Adamant: return "Adamant";
		case Nature::Bashful: return "Bashful";
		case Nature::Bold: return "Bold";
		case Nature::Brave: return "Brave";
		case Nature::Calm: return "Calm";
		case Nature::Careful: return "Careful";
		case Nature::Docile: return "Docile";
		case Nature::Gentle: return "Gentle";
		case Nature::Hardy: return "Hardy";
		case Nature::Hasty: return "Hasty";
		case Nature::Impish: return "Impish";
		case Nature::Jolly: return "Jolly";
		case Nature::Lax: return "Lax";
		case Nature::Lonely: return "Lonely";
		case Nature::Mild: return "Mild";
		case Nature::Modest: return "Modest";
		case Nature::Naive: return "Naive";
		case Nature::Naughty: return "Naughty";
		case Nature::Quiet: return "Quiet";
		case Nature::Quirky: return "Quirky";
		case Nature::Rash: return "Rash";
		case Nature::Relaxed: return "Relaxed";
		case Nature::Sassy: return "Sassy";
		case Nature::Serious: return "Serious";
		case Nature::Timid: return "Timid";
	}
}

template<>
Nature from_string<Nature>(std::string_view const str) {
	static std::map<std::string_view, Nature> const converter {
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
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Nature", str);
	}
}

}	// namespace technicalmachine
