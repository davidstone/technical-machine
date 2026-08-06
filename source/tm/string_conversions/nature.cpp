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
		case Nature::Adamant: return "Adamant"_sv;
		case Nature::Bashful: return "Bashful"_sv;
		case Nature::Bold: return "Bold"_sv;
		case Nature::Brave: return "Brave"_sv;
		case Nature::Calm: return "Calm"_sv;
		case Nature::Careful: return "Careful"_sv;
		case Nature::Docile: return "Docile"_sv;
		case Nature::Gentle: return "Gentle"_sv;
		case Nature::Hardy: return "Hardy"_sv;
		case Nature::Hasty: return "Hasty"_sv;
		case Nature::Impish: return "Impish"_sv;
		case Nature::Jolly: return "Jolly"_sv;
		case Nature::Lax: return "Lax"_sv;
		case Nature::Lonely: return "Lonely"_sv;
		case Nature::Mild: return "Mild"_sv;
		case Nature::Modest: return "Modest"_sv;
		case Nature::Naive: return "Naive"_sv;
		case Nature::Naughty: return "Naughty"_sv;
		case Nature::Quiet: return "Quiet"_sv;
		case Nature::Quirky: return "Quirky"_sv;
		case Nature::Rash: return "Rash"_sv;
		case Nature::Relaxed: return "Relaxed"_sv;
		case Nature::Sassy: return "Sassy"_sv;
		case Nature::Serious: return "Serious"_sv;
		case Nature::Timid: return "Timid"_sv;
	}
}

export template<>
constexpr auto from_string<Nature>(containers::string_view const str) -> Nature {
	static constexpr auto converter = containers::basic_flat_map(
		containers::assume_sorted_unique,
		containers::to_array<containers::map_value_type<containers::string_view, Nature>>({
			{"adamant"_sv, Nature::Adamant},
			{"bashful"_sv, Nature::Bashful},
			{"bold"_sv, Nature::Bold},
			{"brave"_sv, Nature::Brave},
			{"calm"_sv, Nature::Calm},
			{"careful"_sv, Nature::Careful},
			{"docile"_sv, Nature::Docile},
			{"gentle"_sv, Nature::Gentle},
			{"hardy"_sv, Nature::Hardy},
			{"hasty"_sv, Nature::Hasty},
			{"impish"_sv, Nature::Impish},
			{"jolly"_sv, Nature::Jolly},
			{"lax"_sv, Nature::Lax},
			{"lonely"_sv, Nature::Lonely},
			{"mild"_sv, Nature::Mild},
			{"modest"_sv, Nature::Modest},
			{"naive"_sv, Nature::Naive},
			{"naughty"_sv, Nature::Naughty},
			{"quiet"_sv, Nature::Quiet},
			{"quirky"_sv, Nature::Quirky},
			{"rash"_sv, Nature::Rash},
			{"relaxed"_sv, Nature::Relaxed},
			{"sassy"_sv, Nature::Sassy},
			{"serious"_sv, Nature::Serious},
			{"timid"_sv, Nature::Timid}
		})
	);
	auto const converted = fixed_capacity_lowercase_and_digit_string<7_bi>(str);
	auto const result = containers::lookup(converter, converted);
	if (!result) {
		throw InvalidFromStringConversion("Nature"_sv, str);
	}
	return *result;
}

} // namespace technicalmachine
