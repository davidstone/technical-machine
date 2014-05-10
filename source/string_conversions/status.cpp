// Status string functions
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

#include "status.hpp"
#include "invalid_string_conversion.hpp"

#include <map>
#include <string>

#include "../status.hpp"

namespace technicalmachine {

std::string to_string(Status::Statuses const name) {
	static std::string const status_name [] = {
		"No status",
		"Burn",
		"Freeze",
		"Paralysis",
		"Poison",
		"Toxic",
		"Sleep",
		"Rest",
		"END_STATUS"
	};
	return status_name [name];
}

template<>
Status::Statuses from_string(std::string const & str) {
	static std::map <std::string, Status::Statuses> const converter {
		{ "No status", Status::clear },
		{ "Burn", Status::burn },
		{ "Freeze", Status::freeze },
		{ "Paralysis", Status::paralysis },
		{ "Poison", Status::poison },
		{ "Toxic", Status::poison_toxic },
		{ "Sleep", Status::sleep },
		{ "Rest", Status::sleep_rest },
		{ "END_STATUS", Status::END }
	};
	auto const it = converter.find (str);
	if (it != converter.end ())
		return it->second;
	else
		throw InvalidFromStringConversion ("Status", str);
}

}	// namespace technicalmachine
