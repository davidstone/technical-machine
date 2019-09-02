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

#include <tm/string_conversions/team.hpp>

#include <tm/team.hpp>

namespace technicalmachine {

auto to_string(Team const & team, bool const include_owner) -> containers::string {
	containers::string output;
	if (include_owner) {
		output = containers::concatenate<containers::string>(std::move(output), team.who(), std::string_view("'s team:\n"));
	}
	for (auto const & member : team.all_pokemon()) {
		output = containers::concatenate<containers::string>(
			std::move(output),
			to_string(member),
			containers::single_element_range('\n')
		);
	}
	return output;
}

}	// namespace technicalmachine
