// Thrown if an invalid team file is detected
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/pokemon_online/invalid_team_file.hpp>

#include <containers/algorithms/concatenate.hpp>

namespace technicalmachine {
namespace po {

InvalidTeamFile::InvalidTeamFile(std::string const & message):
	std::runtime_error(message)
{
}

InvalidTeamFile::InvalidTeamFile(std::string_view const expected, std::string_view const received):
	InvalidTeamFile(containers::concatenate<std::string>(std::string_view("Expected a field labeled "), expected, std::string_view(" but got "), received))
{
}

}	// namespace po
}	// namespace technicalmachine
