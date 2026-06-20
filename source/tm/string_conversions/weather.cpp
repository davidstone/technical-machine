// Copyright David Stone 2021.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(Weather const weather) -> containers::string_view {
	switch (weather) {
		case Weather::clear: return "Clear"_s;
		case Weather::hail: return "Hail"_s;
		case Weather::sand: return "Sand"_s;
		case Weather::sun: return "Sun"_s;
		case Weather::rain: return "Rain"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Weather {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Weather>>({
			{"clear"_s, Weather::clear},
			{"hail"_s, Weather::hail},
			{"none"_s, Weather::clear},
			{"rain"_s, Weather::rain},
			{"raindance"_s, Weather::rain},
			{"sand"_s, Weather::sand},
			{"sandstorm"_s, Weather::sand},
			{"sun"_s, Weather::sun},
			{"sunnyday"_s, Weather::sun},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Weather"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
