// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/string_conversions/ability.hpp>
#include <tm/string_conversions/gender.hpp>
#include <tm/string_conversions/item.hpp>
#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/nature.hpp>
#include <tm/string_conversions/species.hpp>
#include <tm/string_conversions/status.hpp>

#include <stdexcept>

namespace technicalmachine {

struct InvalidSimulatorConversion : std::logic_error {
	InvalidSimulatorConversion(auto original, auto result):
		std::logic_error(std::string(to_string(original)) + " is seen as " + std::string(to_string(result))) {
	}
};

}	// namespace technicalmachine

