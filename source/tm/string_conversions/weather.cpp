// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string_view>

export module tm.string_conversions.weather;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.weather;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto to_string(Weather const weather) -> std::string_view {
	switch (weather) {
		case Weather::clear: return "Clear";
		case Weather::hail: return "Hail";
		case Weather::sand: return "Sand";
		case Weather::sun: return "Sun";
		case Weather::rain: return "Rain";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Weather {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Weather>>({
			{"clear", Weather::clear},
			{"hail", Weather::hail},
			{"none", Weather::clear},
			{"rain", Weather::rain},
			{"raindance", Weather::rain},
			{"sand", Weather::sand},
			{"sandstorm", Weather::sand},
			{"sun", Weather::sun},
			{"sunnyday", Weather::sun},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Weather", str);
	}
	return *result;
}

} // namespace technicalmachine
