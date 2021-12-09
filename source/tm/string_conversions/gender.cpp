// Gender string conversions
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/gender.hpp>

#include <tm/string_conversions/conversion.hpp>
#include <tm/string_conversions/invalid_string_conversion.hpp>
#include <tm/string_conversions/lowercase_alphanumeric.hpp>

#include <tm/gender.hpp>

#include <containers/array.hpp>
#include <containers/begin_end.hpp>
#include <containers/flat_map.hpp>
#include <containers/lookup.hpp>

namespace technicalmachine {

auto to_string(Gender const gender) -> std::string_view {
	switch (gender) {
		case Gender::female: return "Female";
		case Gender::male: return "Male";
		case Gender::genderless: return "Genderless";
	}
}

template<>
auto from_string(std::string_view const str) -> Gender {
	constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Gender>>({
			{ "female", Gender::female },
			{ "genderless", Gender::genderless },
			{ "male", Gender::male },
			{ "nogender", Gender::genderless },
			{ "none", Gender::genderless },
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<10>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Gender", str);
	}
	return *result;
}

}	// namespace technicalmachine
