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

#include <tm/clients/timestamp.hpp>

#include <cstring>
#include <ctime>

namespace technicalmachine {

std::string timestamp() {
	std::string result;
	result.resize(std::strlen("2000-01-01 12:34:56"));
	std::time_t const current_time = std::time(nullptr);
	tm const * const timeptr = std::localtime(&current_time);
	std::strftime(result.data(), result.size(), "%Y-%m-%d %H:%M:%S", timeptr);
	return result;
}

}	// namespace technicalmachine
