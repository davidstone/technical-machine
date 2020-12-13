// Exception if an invalid team file extension is given
// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <tm/clients/invalid_team_file_format.hpp>

namespace technicalmachine {

InvalidTeamFileFormat::InvalidTeamFileFormat(std::filesystem::path const & file):
	std::logic_error("Attempted to load " + file.string() + ".") {
}

}	// namespace technicalmachine
