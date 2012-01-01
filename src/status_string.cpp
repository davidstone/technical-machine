// Status string functions
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

#include "status.hpp"

#include <map>
#include <string>

namespace technicalmachine {

std::string Status::to_string (Statuses name) {
	static std::string const status_name [] = {
		"No status",
		"Burn",
		"Freeze",
		"Paralysis",
		"Poison",
		"Toxic",
		"Rest",
		"Sleep",
		"END_STATUS"
	};
	return status_name [name];
}

std::string Status::to_string () const {
	return to_string (name);
}

Status::Statuses Status::from_string (std::string const & str) {
	static std::map <std::string, Statuses> const converter {
		{ "No status", NO_STATUS },
		{ "Burn", BURN },
		{ "Freeze", FREEZE },
		{ "Paralysis", PARALYSIS },
		{ "Poison", POISON },
		{ "Toxic", POISON_TOXIC },
		{ "Rest", REST },
		{ "Sleep", SLEEP },
		{ "END_STATUS", END_STATUS }
	};
	return converter.find (str)->second;
}

}	// namespace technicalmachine
