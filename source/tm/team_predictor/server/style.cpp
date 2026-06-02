// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.team_predictor.server.style;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;

import containers;
import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace containers::string_literals;

export enum class Style { random, most_likely };

export constexpr auto to_string(Style const style) -> std::string_view {
	switch (style) {
		case Style::random:
			return "random"_s;
		case Style::most_likely:
			return "most likely"_s;
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Style {
	if (str == "random"_s) {
		return Style::random;
	} else if (str == "most likely"_s) {
		return Style::most_likely;
	} else {
		throw InvalidFromStringConversion("Style", str);
	}
}

} // namespace technicalmachine

template<>
constexpr auto numeric_traits::min_value<technicalmachine::Style> = technicalmachine::Style::random;

template<>
constexpr auto numeric_traits::max_value<technicalmachine::Style> = technicalmachine::Style::most_likely;
