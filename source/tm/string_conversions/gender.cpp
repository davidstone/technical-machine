// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string_view>

export module tm.string_conversions.gender;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.gender;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto to_string(Gender const gender) -> std::string_view {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::male: return "Male";
		case Gender::genderless: return "Genderless";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Gender {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Gender>>({
			{"female", Gender::female},
			{"genderless", Gender::genderless},
			{"male", Gender::male},
			{"nogender", Gender::genderless},
			{"none", Gender::genderless},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<10_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Gender", str);
	}
	return *result;
}

} // namespace technicalmachine
