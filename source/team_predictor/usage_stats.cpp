// Load stats such as Pokemon usage stats
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

#include "usage_stats.hpp"

#include "../invalid_settings_file.hpp"

#include "../pokemon/species.hpp"

#include "../string_conversions/pokemon.hpp"

#include <boost/lexical_cast.hpp>

#include <fstream>
#include <string>

namespace technicalmachine {

namespace {

template<typename T>
auto load_stats_from_file(std::filesystem::path const & file_name) {
	containers::array<T, number_of_species> overall = {};
	auto file = std::ifstream(file_name);
	if (!file.is_open()) {
		throw InvalidSettingsFile(file_name, InvalidSettingsFile::does_not_exist);
	}
	std::string line;
	while(getline(file, line)) {
		constexpr char delimiter = '\t';
		auto const position = line.find(delimiter);
		auto const species = from_string<Species>(line.substr(0, position));
		auto const value = boost::lexical_cast<T>(line.substr(position + 1));
		overall[species] = value;
	}
	return overall;
}

}	// namespace

UsageStats::UsageStats(std::filesystem::path const & usage_stats_directory):
	m_overall(load_stats_from_file<unsigned>(usage_stats_directory / "usage.txt")),
	m_lead(load_stats_from_file<float>(usage_stats_directory / "lead.txt")),
	m_detailed(usage_stats_directory / "detailed.xml"),
	m_multiplier(m_overall, usage_stats_directory / "teammate.txt")
{
}

}	// namespace technicalmachine
