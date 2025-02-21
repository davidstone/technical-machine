// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string_view>

export module tm.team_predictor.server.style;

export import tm.string_conversions.from_string;
import tm.string_conversions.invalid_string_conversion;

import numeric_traits;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;

export enum class Style { random, most_likely };

export constexpr auto to_string(Style const style) -> std::string_view {
	switch (style) {
		case Style::random:
			return "random"sv;
		case Style::most_likely:
			return "most likely"sv;
	}
}

export template<>
constexpr auto from_string(std::string_view const str) -> Style {
	if (str == "random"sv) {
		return Style::random;
	} else if (str == "most likely"sv) {
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
