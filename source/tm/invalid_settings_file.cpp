// Exception class for settings files that are incorrect
// Copyright (C) 2018 David Stone
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

#include <containers/string.hpp>

#include <string>

namespace technicalmachine {
namespace {

std::string to_string(InvalidSettingsFile::Problem const problem) {
	switch (problem) {
		case InvalidSettingsFile::Problem::does_not_exist: return " does not exist.";
		case InvalidSettingsFile::Problem::too_long: return " is too long.";
		case InvalidSettingsFile::Problem::too_short: return " is too short.";
		case InvalidSettingsFile::Problem::invalid_data: return " contains invalid data.";
	}
}
}	// namespace

InvalidSettingsFile::InvalidSettingsFile(std::filesystem::path const & file_name, Problem const problem):
	std::runtime_error(file_name.string() + to_string(problem)) {
}

}	// namespace technicalmachine
