// Thrown if an invalid team file is detected
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <stdexcept>
#include <string>

namespace technicalmachine {
namespace po {

struct InvalidTeamFile : std::runtime_error {
	InvalidTeamFile(std::string const & message);
	InvalidTeamFile(std::string const & expected, std::string const & received);
};

}	// namespace po
}	// namespace technicalmachine
