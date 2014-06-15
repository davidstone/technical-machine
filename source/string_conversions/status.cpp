// Status string functions
// Copyright (C) 2014 David Stone
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

#include "status.hpp"
#include "invalid_string_conversion.hpp"

#include "../status.hpp"

#include <bounded_integer/array.hpp>

#include <map>
#include <string>

namespace technicalmachine {

std::string to_string(Statuses const name) {
	static auto const status_name = bounded::make_array<std::string>(
		"No status",
		"Burn",
		"Freeze",
		"Paralysis",
		"Poison",
		"Toxic",
		"Sleep",
		"Rest",
		"END_STATUS"
	);
	return status_name.at(name);
}

template<>
Statuses from_string(std::string const & str) {
	static std::map <std::string, Statuses> const converter {
		{ "No status", Statuses::clear },
		{ "Burn", Statuses::burn },
		{ "Freeze", Statuses::freeze },
		{ "Paralysis", Statuses::paralysis },
		{ "Poison", Statuses::poison },
		{ "Toxic", Statuses::poison_toxic },
		{ "Sleep", Statuses::sleep },
		{ "Rest", Statuses::sleep_rest },
		{ "END_STATUS", Statuses::END }
	};
	auto const it = converter.find (str);
	if (it != converter.end()) {
		return it->second;
	} else {
		throw InvalidFromStringConversion ("Status", str);
	}
}

}	// namespace technicalmachine
