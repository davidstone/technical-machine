// Connect to an arbitrary Pokemon sim
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

// What follows are generic functions and data for connection to any Pokemon
// sim. Specific functions and data are found in the respective sim's folder.

#include "client.hpp"

#include "../settings_file.hpp"

namespace technicalmachine {

void Client::print_with_time_stamp (std::ostream & stream, std::string const & message) const {
	stream << time_stamp() + " " + message + "\n";
}

std::string Client::time_stamp() const {
	// There does not appear to be an easy way to format the current time with
	// a format string. This seems like a major limitation of boost::date_time
	// and / or boost::posix_time, as well as the std header chrono.
	std::string result;
	if (!time_format.empty ()) {
		// probably_big_enough is a guess at how big the time stamp will be.
		// It is OK if it is wrong.
		constexpr unsigned probably_big_enough = 30;
		result.resize (probably_big_enough);
		time_t current_time = time (nullptr);
		tm * timeptr = localtime (&current_time);
		while (strftime (&result [0], result.size (), time_format.c_str(), timeptr) == 0)
			result.resize (result.size () * 2);
	}
	return result;
}

Settings Client::load_settings() {
	Settings settings;
	time_format = settings.time_format;
	return settings;
}

void Client::reload_settings() {
	m_evaluation_constants.load();
}

DetailedStats const & Client::detailed() const {
	return detailed_stats;
}

Evaluate const & Client::evaluation_constants() const {
	return m_evaluation_constants;
}

}	// namespace technicalmachine
