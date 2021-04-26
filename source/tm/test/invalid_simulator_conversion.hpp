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

#include <containers/algorithms/concatenate.hpp>

#include <stdexcept>
#include <string>
#include <string_view>

namespace technicalmachine {

struct InvalidSimulatorConversion : std::logic_error {
	InvalidSimulatorConversion(auto original, auto result):
		std::logic_error(containers::concatenate<std::string>(to_string(original), std::string_view(" is seen as "), to_string(result)))
	{
	}
};

}	// namespace technicalmachine

