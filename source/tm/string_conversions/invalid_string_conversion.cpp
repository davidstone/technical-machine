// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <containers/algorithms/concatenate.hpp>

#include <string>
#include <string_view>

namespace technicalmachine {

using namespace std::string_view_literals;

InvalidFromStringConversion::InvalidFromStringConversion(std::string_view const target, std::string_view const requested):
	std::runtime_error(containers::concatenate<std::string>("Invalid conversion from "sv, requested, " to type "sv, target))
{
}

} // namespace technicalmachine
