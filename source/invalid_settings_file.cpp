// Exception class for settings files that are incorrect
// Copyright (C) 2012 David Stone
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

#include "invalid_settings_file.hpp"

namespace technicalmachine {
namespace {

std::string to_string(InvalidSettingsFile::Problem const problem) {
	static const std::string text [] = {
		"does not exist",
		"is too long",
		"is too short",
		"contains invalid data"
	};
	return text[problem];
}
}	// unnamed namespace

InvalidSettingsFile::InvalidSettingsFile(std::string const & file_name, Problem const problem):
	std::runtime_error(file_name + " " + to_string(problem) + ".") {
}

}	// namespace technicalmachine
