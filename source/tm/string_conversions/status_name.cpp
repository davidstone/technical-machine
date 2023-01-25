// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string_view>

export module tm.string_conversions.status_name;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.status.status_name;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto to_string(StatusName const status) -> std::string_view {
	switch (status) {
		case StatusName::clear: return "No status";
		case StatusName::burn: return "Burn";
		case StatusName::freeze: return "Freeze";
		case StatusName::paralysis: return "Paralysis";
		case StatusName::poison: return "Poison";
		case StatusName::toxic: return "Toxic";
		case StatusName::sleep: return "Sleep";
		case StatusName::rest: return "Rest";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> StatusName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, StatusName>>({
			{"burn", StatusName::burn},
			{"freeze", StatusName::freeze},
			{"nostatus", StatusName::clear},
			{"paralysis", StatusName::paralysis},
			{"poison", StatusName::poison},
			{"rest", StatusName::rest},
			{"sleep", StatusName::sleep},
			{"toxic", StatusName::toxic},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Status", str);
	}
	return *result;
}

} // namespace technicalmachine
