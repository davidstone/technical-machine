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

#ifndef INVALID_SETTINGS_FILE_HPP_
#define INVALID_SETTINGS_FILE_HPP_

#include <stdexcept>
#include <string>

namespace technicalmachine {

class InvalidSettingsFile : public std::runtime_error {
public:
	enum Problem {
		does_not_exist,
		too_long,
		too_short,
		invalid_data
	};
	InvalidSettingsFile(std::string const & file_name, Problem const problem);
};

}	// namespace technicalmachine
#endif	// INVALID_SETTINGS_FILE_HPP_
