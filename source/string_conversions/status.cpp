// Status string functions
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

#include "status.hpp"
#include "invalid_string_conversion.hpp"

#include "../status.hpp"

#include <map>

namespace technicalmachine {

std::string_view to_string(Statuses const status) {
	switch (status) {
		case Statuses::clear: return "No status";
		case Statuses::burn: return "Burn";
		case Statuses::freeze: return "Freeze";
		case Statuses::paralysis: return "Paralysis";
		case Statuses::poison: return "Poison";
		case Statuses::poison_toxic: return "Toxic";
		case Statuses::sleep: return "Sleep";
		case Statuses::sleep_rest: return "Rest";
	}
}

template<>
Statuses from_string(std::string_view const str) {
	static std::map<std::string_view, Statuses> const converter {
		{ "No status", Statuses::clear },
		{ "Burn", Statuses::burn },
		{ "Freeze", Statuses::freeze },
		{ "Paralysis", Statuses::paralysis },
		{ "Poison", Statuses::poison },
		{ "Toxic", Statuses::poison_toxic },
		{ "Sleep", Statuses::sleep },
		{ "Rest", Statuses::sleep_rest }
	};
	auto const it = converter.find(str);
	if (it != end(converter)) {
		return it->second;
	} else {
		throw InvalidFromStringConversion("Status", str);
	}
}

}	// namespace technicalmachine
