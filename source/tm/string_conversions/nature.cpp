// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string_view>

export module tm.string_conversions.nature;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;
import tm.string_conversions.lowercase_alphanumeric;

import tm.stat.nature;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

export constexpr auto to_string(Nature const nature) -> std::string_view {
	switch (nature) {
		case Nature::Adamant: return "Adamant";
		case Nature::Bashful: return "Bashful";
		case Nature::Bold: return "Bold";
		case Nature::Brave: return "Brave";
		case Nature::Calm: return "Calm";
		case Nature::Careful: return "Careful";
		case Nature::Docile: return "Docile";
		case Nature::Gentle: return "Gentle";
		case Nature::Hardy: return "Hardy";
		case Nature::Hasty: return "Hasty";
		case Nature::Impish: return "Impish";
		case Nature::Jolly: return "Jolly";
		case Nature::Lax: return "Lax";
		case Nature::Lonely: return "Lonely";
		case Nature::Mild: return "Mild";
		case Nature::Modest: return "Modest";
		case Nature::Naive: return "Naive";
		case Nature::Naughty: return "Naughty";
		case Nature::Quiet: return "Quiet";
		case Nature::Quirky: return "Quirky";
		case Nature::Rash: return "Rash";
		case Nature::Relaxed: return "Relaxed";
		case Nature::Sassy: return "Sassy";
		case Nature::Serious: return "Serious";
		case Nature::Timid: return "Timid";
	}
}

export template<>
constexpr auto from_string<Nature>(std::string_view const str) -> Nature {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<std::string_view, Nature>>({
			{"adamant", Nature::Adamant},
			{"bashful", Nature::Bashful},
			{"bold", Nature::Bold},
			{"brave", Nature::Brave},
			{"calm", Nature::Calm},
			{"careful", Nature::Careful},
			{"docile", Nature::Docile},
			{"gentle", Nature::Gentle},
			{"hardy", Nature::Hardy},
			{"hasty", Nature::Hasty},
			{"impish", Nature::Impish},
			{"jolly", Nature::Jolly},
			{"lax", Nature::Lax},
			{"lonely", Nature::Lonely},
			{"mild", Nature::Mild},
			{"modest", Nature::Modest},
			{"naive", Nature::Naive},
			{"naughty", Nature::Naughty},
			{"quiet", Nature::Quiet},
			{"quirky", Nature::Quirky},
			{"rash", Nature::Rash},
			{"relaxed", Nature::Relaxed},
			{"sassy", Nature::Sassy},
			{"serious", Nature::Serious},
			{"timid", Nature::Timid}
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<7_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Nature", str);
	}
	return *result;
}

} // namespace technicalmachine
