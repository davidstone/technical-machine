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

#include <tm/string_conversions/status.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/status.hpp>

#include <containers/array/array.hpp>
#include <containers/flat_map.hpp>

namespace technicalmachine {

std::string_view to_string(Statuses const status) {
	switch (status) {
		case Statuses::clear: return "No status";
		case Statuses::burn: return "Burn";
		case Statuses::freeze: return "Freeze";
		case Statuses::paralysis: return "Paralysis";
		case Statuses::poison: return "Poison";
		case Statuses::toxic: return "Toxic";
		case Statuses::sleep: return "Sleep";
		case Statuses::rest: return "Rest";
	}
}

template<>
Statuses from_string(std::string_view const str) {
	using Storage = containers::array<containers::map_value_type<std::string_view, Statuses>, 8>;
	static constexpr auto converter = containers::basic_flat_map<Storage>(
		containers::assume_sorted_unique,
		Storage{{
			{ "burn", Statuses::burn },
			{ "freeze", Statuses::freeze },
			{ "nostatus", Statuses::clear },
			{ "paralysis", Statuses::paralysis },
			{ "poison", Statuses::poison },
			{ "rest", Statuses::rest },
			{ "sleep", Statuses::sleep },
			{ "toxic", Statuses::toxic },
		}}
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9>(str);
	auto const it = converter.find(converted);
	if (it != end(converter)) {
		return it->mapped();
	} else {
		throw InvalidFromStringConversion("Status", str);
	}
}

}	// namespace technicalmachine
