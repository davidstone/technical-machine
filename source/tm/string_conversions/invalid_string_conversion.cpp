// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/string_conversions/invalid_string_conversion.hpp>

#include <string>

namespace technicalmachine {

InvalidFromStringConversion::InvalidFromStringConversion(std::string_view const target, std::string_view const requested):
	std::runtime_error("Invalid conversion from " + std::string(requested) + " to type " + std::string(target))
{
}

}	// namespace technicalmachine
