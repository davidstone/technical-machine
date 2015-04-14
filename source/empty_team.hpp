// Exception if operations are performed on an empty team
// Copyright (C) 2015 David Stone
//
// This file is part of Technical Machine.
//
// Technical Machine is free software: you can redistribute it and / or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.

#ifndef EMPTY_TEAM_HPP_
#define EMPTY_TEAM_HPP_

#include <boost/filesystem/path.hpp>

#include <stdexcept>

namespace technicalmachine {

struct EmptyTeam : std::logic_error {
	EmptyTeam(boost::filesystem::path const & file, unsigned const line):
		std::logic_error("Attempted operation on a team of size 0 at: " + file.string() + ": " + std::to_string(line) + ".") {
	}
};
}	// namespace technicalmachine
#endif	// EMPTY_TEAM_HPP_
