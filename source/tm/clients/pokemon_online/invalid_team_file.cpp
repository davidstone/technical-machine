// Thrown if an invalid team file is detected
// Copyright (C) 2013 David Stone
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

#include <tm/clients/pokemon_online/invalid_team_file.hpp>

namespace technicalmachine {
namespace po {

InvalidTeamFile::InvalidTeamFile(std::string const & expected, std::string const & received):
	std::runtime_error("Expected a field labeled " + expected + " but got " + received) {
}

}	// namespace po
}	// namespace technicalmachine
