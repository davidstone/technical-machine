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
#include <cstddef>
#include <fstream>
#include <string>
#include <boost/lexical_cast.hpp>

#include "../invalid_settings_file.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

namespace technicalmachine {

namespace {

template<typename T>
std::array<T, max_species> load_stats_from_file (std::string const & file_name) {
	std::array<T, max_species> overall;
	std::ifstream file (file_name);
	std::string line;
	unsigned n = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (n >= max_species)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_long);
		overall [n] = boost::lexical_cast<T> (line);
		++n;
	}
	if (n != max_species)
		throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_short);
	return overall;
}

void species_clause (float multiplier [max_species] [max_species]) {
	for (unsigned a = 0; a != max_species; ++a) {
		for (unsigned b = 0; b != max_species; ++b) {
			Species const first = static_cast <Species> (a);
			Species const second = static_cast <Species> (b);
			// Species clause or replaced with other value later
			multiplier [a][b] = is_alternate_form(first, second) ? 0 : -1;
		}
		multiplier [a] [a] = 0;			// Species clause
	}
}

void load_listed_multipliers (float multiplier [max_species] [max_species], std::array<unsigned, max_species> const & overall, std::array<unsigned, max_species> & unaccounted, unsigned total) {
	std::string const file_name = "settings/teammate.txt";
	std::ifstream file (file_name);
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		constexpr char delimiter = '\t';
		size_t const x = line.find (delimiter);
		unsigned const member = boost::lexical_cast<unsigned> (line.substr (0, x));
		size_t const y = line.find (delimiter, x + 1);
		unsigned const ally = boost::lexical_cast<unsigned> (line.substr (x + 1, y - x - 1));
		if (member >= max_species or ally >= max_species)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::invalid_data);
		unsigned const number_used_with = boost::lexical_cast<unsigned> (line.substr (y + 1));
		unaccounted [member] -= number_used_with;
		float const per_cent_used_with = static_cast <float> (number_used_with) / overall [member];
		float const per_cent_used = static_cast <float> (overall [ally]) / total;
		multiplier [member] [ally] = per_cent_used_with / per_cent_used;
	}
}

void estimate_remaining_multipliers (float multiplier [max_species] [max_species], std::array<unsigned, max_species> const & overall, std::array<unsigned, max_species> const & unaccounted) {
	for (unsigned a = 0; a != max_species; ++a) {
		if (overall [a] != 0) {
			for (float & value : multiplier [a]) {
				if (value == -1) {
					// Take the total number of remaining Pokemon not accounted
					// for and assume they're distributed the same as their
					// overall distribution among all Pokemon not on the list
					// of team mate stats. This is the same as giving all those
					// Pokemon the same multiplier. Reality probably has very
					// low usage Pokemon more evenly distributed and high usage
					// Pokemon that don't appear with a much lower multiplier.
					// There are probably cases where I can prove that certain
					// Pokemon are lower than my current method would suggest,
					// so I should look into limiting the maximum multiplier.
					value = unaccounted [a] / (overall [a] * 5.0);
				}
			}
		}
		else {
			for (float & value : multiplier [a])
				// 1 is superior to 0 because if they use an unused Pokemon,
				// this will have no effect instead of making everything
				// equally 0
				value = 1;
		}
	}
}

}	// unnamed namespace

std::array<unsigned, max_species> overall_stats () {
	return load_stats_from_file<unsigned> ("settings/usage.txt");
}

std::array<float, max_species> lead_stats () {
	return load_stats_from_file<float> ("settings/lead.txt");
}

void team_stats (std::array<unsigned, max_species> const & overall, unsigned const total, float multiplier [max_species][max_species]) {

	species_clause (multiplier);

	// There are 5 other Pokemon on a team for each individual Pokemon.
	// Therefore, if I've seen a Pokemon with n usages, there are 5 * n other
	// Pokemon on the team. Subtract the known number of usages from this
	// number until all known usages are gone. Then, assume the distribution of
	// Pokemon not on the team mate stats is equal to the relative overall
	// distribution and divide up all remaining usages proportionally.
	std::array<unsigned, max_species> unaccounted;
	for (unsigned n = 0; n != max_species; ++n)
		unaccounted [n] = overall [n] * 5;

	load_listed_multipliers (multiplier, overall, unaccounted, total);
	estimate_remaining_multipliers (multiplier, overall, unaccounted);
}

}	// namespace technicalmachine
