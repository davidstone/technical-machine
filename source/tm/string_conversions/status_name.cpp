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
		case StatusName::clear: return "No status"_sv;
		case StatusName::burn: return "Burn"_sv;
		case StatusName::freeze: return "Freeze"_sv;
		case StatusName::paralysis: return "Paralysis"_sv;
		case StatusName::poison: return "Poison"_sv;
		case StatusName::toxic: return "Toxic"_sv;
		case StatusName::sleep: return "Sleep"_sv;
		case StatusName::rest: return "Rest"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> StatusName {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, StatusName>>({
			{"burn"_sv, StatusName::burn},
			{"freeze"_sv, StatusName::freeze},
			{"nostatus"_sv, StatusName::clear},
			{"paralysis"_sv, StatusName::paralysis},
			{"poison"_sv, StatusName::poison},
			{"rest"_sv, StatusName::rest},
			{"sleep"_sv, StatusName::sleep},
			{"toxic"_sv, StatusName::toxic},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Status"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
