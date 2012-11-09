// Load stats such as Pokemon usage stats
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

#include "load_stats.hpp"

#include <array>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "../invalid_settings_file.hpp"

#include "../pokemon/species.hpp"

#include "../string_conversions/conversion.hpp"

namespace technicalmachine {

namespace {

template<typename T>
std::array<T, number_of_species> load_stats_from_file (std::string const & file_name) {
	std::array<T, number_of_species> overall;
	std::ifstream file(file_name);
	if (!file.is_open()) {
		throw InvalidSettingsFile(file_name, InvalidSettingsFile::does_not_exist);
	}
	std::string line;
	unsigned n = 0;
	while(getline(file, line)) {
		if (n >= number_of_species)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_long);
		overall [n] = boost::lexical_cast<T> (line);
		++n;
	}
	if (n != number_of_species)
		throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_short);
	return overall;
}

}	// unnamed namespace

std::array<unsigned, number_of_species> overall_stats () {
	return load_stats_from_file<unsigned> ("settings/Generation 4/OU/usage.txt");
}

std::array<float, number_of_species> lead_stats () {
	return load_stats_from_file<float> ("settings/Generation 4/OU/lead.txt");
}

}	// namespace technicalmachine
