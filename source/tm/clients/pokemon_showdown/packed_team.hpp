// Copyright (C) 2019 David Stone
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

#pragma once

#include <tm/generation.hpp>
#include <tm/team.hpp>

#include <containers/string.hpp>

#include <string_view>

namespace technicalmachine {

namespace ps {

auto to_packed_format(Team const &) -> containers::string;
auto packed_format_to_team(std::string_view, Generation) -> Team;
auto packed_format_to_team(std::string_view, Generation, TeamSize) -> Team;

}	// namespace ps
}	// namespace technicalmachine
