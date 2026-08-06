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
		case Weather::clear: return "Clear"_sv;
		case Weather::hail: return "Hail"_sv;
		case Weather::sand: return "Sand"_sv;
		case Weather::sun: return "Sun"_sv;
		case Weather::rain: return "Rain"_sv;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Weather {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Weather>>({
			{"clear"_sv, Weather::clear},
			{"hail"_sv, Weather::hail},
			{"none"_sv, Weather::clear},
			{"rain"_sv, Weather::rain},
			{"raindance"_sv, Weather::rain},
			{"sand"_sv, Weather::sand},
			{"sandstorm"_sv, Weather::sand},
			{"sun"_sv, Weather::sun},
			{"sunnyday"_sv, Weather::sun},
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<9_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Weather"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
