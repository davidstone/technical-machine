// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(StatusName const status) -> containers::string_view {
	switch (status) {
		case StatusName::clear: return "No status"_s;
		case StatusName::burn: return "Burn"_s;
		case StatusName::freeze: return "Freeze"_s;
		case StatusName::paralysis: return "Paralysis"_s;
		case StatusName::poison: return "Poison"_s;
		case StatusName::toxic: return "Toxic"_s;
		case StatusName::sleep: return "Sleep"_s;
		case StatusName::rest: return "Rest"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> StatusName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, StatusName>>({
			{"burn"_s, StatusName::burn},
			{"freeze"_s, StatusName::freeze},
			{"nostatus"_s, StatusName::clear},
			{"paralysis"_s, StatusName::paralysis},
			{"poison"_s, StatusName::poison},
			{"rest"_s, StatusName::rest},
			{"sleep"_s, StatusName::sleep},
			{"toxic"_s, StatusName::toxic},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Status"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
