// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.string_conversions.generation;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.generation;

import containers;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export constexpr auto to_string(Generation const generation) -> containers::string_view {
	switch (generation) {
		case Generation::one: return "1"_s;
		case Generation::two: return "2"_s;
		case Generation::three: return "3"_s;
		case Generation::four: return "4"_s;
		case Generation::five: return "5"_s;
		case Generation::six: return "6"_s;
		case Generation::seven: return "7"_s;
		case Generation::eight: return "8"_s;
	}
}

export template<>
constexpr auto from_string(containers::string_view const str) -> Generation {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Generation>>({
			{"1"_s, Generation::one},
			{"2"_s, Generation::two},
			{"3"_s, Generation::three},
			{"4"_s, Generation::four},
			{"5"_s, Generation::five},
			{"6"_s, Generation::six},
			{"7"_s, Generation::seven},
			{"8"_s, Generation::eight},
		})
	);
	auto const result = containers::lookup(converter, str);
	if (!result) {
		throw InvalidFromStringConversion("Generation"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
