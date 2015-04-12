// Construct a timestamp string
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

#include "timestamp.hpp"

#include <ctime>
#include <iostream>

namespace technicalmachine {
namespace {

std::string make_timestamp() {
	#define SAMPLE_OUTPUT "2000-01-01 12:34:56"
	std::string result;
	result.resize(sizeof(SAMPLE_OUTPUT));
	std::time_t const current_time = std::time(nullptr);
	tm const * const timeptr = localtime(&current_time);
	std::strftime(&result.front(), result.size(), "%Y-%m-%d %H:%M:%S", timeptr);
	return result;
}

}	// namespace

std::string const & timestamp() {
	static std::string const value = make_timestamp();
	return value;
}

}	// namespace technicalmachine
