// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(Gender const gender) -> containers::string_view {
	switch (gender) {
		case Gender::female: return "Female"_sv;
		case Gender::male: return "Male"_sv;
		case Gender::genderless: return "Genderless"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Gender {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Gender>>({
			{"female"_sv, Gender::female},
			{"genderless"_sv, Gender::genderless},
			{"male"_sv, Gender::male},
			{"nogender"_sv, Gender::genderless},
			{"none"_sv, Gender::genderless},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<10_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Gender"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
