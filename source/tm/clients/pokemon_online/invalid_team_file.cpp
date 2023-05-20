// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <std_module/prelude.hpp>
#include <string>

export module tm.clients.po.invalid_team_file;

import containers;
import std_module;

namespace technicalmachine::po {

export struct InvalidTeamFile : std::runtime_error {
	explicit InvalidTeamFile(std::string const & message):
		std::runtime_error(message)
	{
	}

	InvalidTeamFile(std::string_view const expected, std::string_view const received):
		InvalidTeamFile(containers::concatenate<std::string>(std::string_view("Expected a field labeled "), expected, std::string_view(" but got "), received))
	{
	}
};


} // namespace technicalmachine::po
