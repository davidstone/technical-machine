// Thrown if an invalid team file is detected
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_online/invalid_team_file.hpp>

namespace technicalmachine {
namespace po {

InvalidTeamFile::InvalidTeamFile(std::string const & message):
	std::runtime_error(message)
{
}

InvalidTeamFile::InvalidTeamFile(std::string const & expected, std::string const & received):
	InvalidTeamFile("Expected a field labeled " + expected + " but got " + received)
{
}

}	// namespace po
}	// namespace technicalmachine
