// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <compare>
#include <string>

export module tm.clients.invalid_team_file_format;

import containers;
import std_module;

namespace technicalmachine {

using namespace std::string_view_literals;

export struct InvalidTeamFileFormat : std::runtime_error {
	explicit InvalidTeamFileFormat(std::filesystem::path const & file):
		std::runtime_error(containers::concatenate<std::string>("Attempted to load "sv, file.string(), "."sv))
	{
	}
};

} // namespace technicalmachine
