// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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
using namespace containers::string_literals;

export constexpr auto to_string(Nature const nature) -> containers::string_view {
	switch (nature) {
		case Nature::Adamant: return "Adamant"_s;
		case Nature::Bashful: return "Bashful"_s;
		case Nature::Bold: return "Bold"_s;
		case Nature::Brave: return "Brave"_s;
		case Nature::Calm: return "Calm"_s;
		case Nature::Careful: return "Careful"_s;
		case Nature::Docile: return "Docile"_s;
		case Nature::Gentle: return "Gentle"_s;
		case Nature::Hardy: return "Hardy"_s;
		case Nature::Hasty: return "Hasty"_s;
		case Nature::Impish: return "Impish"_s;
		case Nature::Jolly: return "Jolly"_s;
		case Nature::Lax: return "Lax"_s;
		case Nature::Lonely: return "Lonely"_s;
		case Nature::Mild: return "Mild"_s;
		case Nature::Modest: return "Modest"_s;
		case Nature::Naive: return "Naive"_s;
		case Nature::Naughty: return "Naughty"_s;
		case Nature::Quiet: return "Quiet"_s;
		case Nature::Quirky: return "Quirky"_s;
		case Nature::Rash: return "Rash"_s;
		case Nature::Relaxed: return "Relaxed"_s;
		case Nature::Sassy: return "Sassy"_s;
		case Nature::Serious: return "Serious"_s;
		case Nature::Timid: return "Timid"_s;
	}
}

export template<>
constexpr auto from_string<Nature>(containers::string_view const str) -> Nature {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Nature>>({
			{"adamant"_s, Nature::Adamant},
			{"bashful"_s, Nature::Bashful},
			{"bold"_s, Nature::Bold},
			{"brave"_s, Nature::Brave},
			{"calm"_s, Nature::Calm},
			{"careful"_s, Nature::Careful},
			{"docile"_s, Nature::Docile},
			{"gentle"_s, Nature::Gentle},
			{"hardy"_s, Nature::Hardy},
			{"hasty"_s, Nature::Hasty},
			{"impish"_s, Nature::Impish},
			{"jolly"_s, Nature::Jolly},
			{"lax"_s, Nature::Lax},
			{"lonely"_s, Nature::Lonely},
			{"mild"_s, Nature::Mild},
			{"modest"_s, Nature::Modest},
			{"naive"_s, Nature::Naive},
			{"naughty"_s, Nature::Naughty},
			{"quiet"_s, Nature::Quiet},
			{"quirky"_s, Nature::Quirky},
			{"rash"_s, Nature::Rash},
			{"relaxed"_s, Nature::Relaxed},
			{"sassy"_s, Nature::Sassy},
			{"serious"_s, Nature::Serious},
			{"timid"_s, Nature::Timid}
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<7_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Nature"_s, str);
	}
	return *result;
}

} // namespace technicalmachine
