// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <string>

export module tm.string_conversions.invalid_string_conversion;

import containers;
import std_module;

namespace technicalmachine {
using namespace std::string_view_literals;

export struct InvalidFromStringConversion : std::runtime_error {
	InvalidFromStringConversion(std::string_view const target, std::string_view const requested):
		std::runtime_error(containers::concatenate<std::string>("Invalid conversion from "sv, requested, " to type "sv, target))
	{
	}
};


} // namespace technicalmachine
