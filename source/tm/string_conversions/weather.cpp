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

export constexpr auto to_string(NormalWeather const weather) -> std::string_view {
	switch (weather) {
		case NormalWeather::clear: return "Clear";
		case NormalWeather::hail: return "Hail";
		case NormalWeather::sand: return "Sand";
		case NormalWeather::sun: return "Sun";
		case NormalWeather::rain: return "Rain";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> NormalWeather {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, NormalWeather>>({
			{"clear", NormalWeather::clear},
			{"hail", NormalWeather::hail},
			{"none", NormalWeather::clear},
			{"rain", NormalWeather::rain},
			{"raindance", NormalWeather::rain},
			{"sand", NormalWeather::sand},
			{"sandstorm", NormalWeather::sand},
			{"sun", NormalWeather::sun},
			{"sunnyday", NormalWeather::sun},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("NormalWeather", str);
	}
	return *result;
}

} // namespace technicalmachine
