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

#include <tm/clients/log_foe_team.hpp>

#include <tm/clients/random_string.hpp>

#include <string>

namespace technicalmachine {
namespace {

std::string get_extension() {
	// TODO: add support for other formats
	return ".sbt";
}

}	// namespace

std::filesystem::path generate_team_file_name(std::mt19937 & random_engine) {
	// Randomly generates a file name in 8.3 format. It then checks to see if
	// that file name already exists. If it does, it randomly generates a new
	// file name, and continues until it generates a name that does not exist.
	// This limits the potential for a race condition down to a 1 / 36^8 chance
	// (about 1 / 2 ^ 41), assuming that another process / thread is also
	// trying to save an 8 character file name with an identical extension at
	// the same time. The result of this is that a team file would not be saved
	// when it should have been, which is not a major issue.
	constexpr unsigned file_name_length = 8;
	std::filesystem::path foe_team_file;
	do {
		foe_team_file = team_file_directory();
		foe_team_file /= random_string(random_engine, file_name_length) + get_extension();
	} while (std::filesystem::exists(foe_team_file));
	return foe_team_file;
}

} // namespace technicalmachine
