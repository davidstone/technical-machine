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

export constexpr auto to_string(Generation const generation) -> std::string_view {
	switch (generation) {
		case Generation::one: return "1";
		case Generation::two: return "2";
		case Generation::three: return "3";
		case Generation::four: return "4";
		case Generation::five: return "5";
		case Generation::six: return "6";
		case Generation::seven: return "7";
		case Generation::eight: return "8";
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Generation {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Generation>>({
			{"1", Generation::one},
			{"2", Generation::two},
			{"3", Generation::three},
			{"4", Generation::four},
			{"5", Generation::five},
			{"6", Generation::six},
			{"7", Generation::seven},
			{"8", Generation::eight},
		})
	);
	auto const result = containers::lookup(converter, str);
	if (!result) {
		throw InvalidFromStringConversion("Generation", str);
	}
	return *result;
}

} // namespace technicalmachine
