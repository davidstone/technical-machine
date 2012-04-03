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
#include <stdexcept>
#include <string>
#include <boost/lexical_cast.hpp>

#include "move.hpp"
#include "pokemon.hpp"
#include "species.hpp"

namespace technicalmachine {

namespace {

class InvalidSettingsFile : public std::runtime_error {
	public:
		enum Problem {
			too_long,
			too_short,
			invalid_data
		};
		static std::string to_string (Problem const problem) {
			static const std::string text [] = {
				"is too long",
				"is too short",
				"contains invalid data"
			};
			return text [problem];
		}
		InvalidSettingsFile (std::string const & file_name, Problem const problem):
			std::runtime_error (file_name + " " + to_string (problem) + ".") {
		}
};

template<typename T>
std::array<T, Species::END> load_stats_from_file (std::string const & file_name) {
	std::array<T, Species::END> overall;
	std::ifstream file (file_name);
	std::string line;
	unsigned n = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		if (n >= Species::END)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_long);
		overall [n] = boost::lexical_cast<T> (line);
		++n;
	}
	if (n != Species::END)
		throw InvalidSettingsFile (file_name, InvalidSettingsFile::too_short);
	return overall;
}

void species_clause (float multiplier [Species::END] [Species::END]) {
	for (unsigned a = 0; a != Species::END; ++a) {
		for (unsigned b = 0; b != Species::END; ++b) {
			Species const first = static_cast <Species> (a);
			Species const second = static_cast <Species> (b);
			if (Pokemon::is_alternate_form (first, second))
				multiplier [a] [b] = 0;	// Species clause
			else
				multiplier [a] [b] = -1;		// Replaced with other value later
		}
		multiplier [a] [a] = 0;			// Species clause
	}
}

void load_listed_multipliers (float multiplier [Species::END] [Species::END], std::array<unsigned, Species::END> const & overall, std::array<unsigned, Species::END> & unaccounted, unsigned total) {
	std::string const file_name = "settings/teammate.txt";
	std::ifstream file (file_name);
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t const x = line.find ('\t');
		unsigned const member = boost::lexical_cast<unsigned> (line.substr (0, x));
		size_t const y = line.find ('\t', x + 1);
		unsigned const ally = boost::lexical_cast<unsigned> (line.substr (x + 1, y - x - 1));
		if (member >= Species::END or ally >= Species::END)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::invalid_data);
		unsigned const number_used_with = boost::lexical_cast<unsigned> (line.substr (y + 1));
		unaccounted [member] -= number_used_with;
		float const per_cent_used_with = static_cast <float> (number_used_with) / overall [member];
		float const per_cent_used = static_cast <float> (overall [ally]) / total;
		multiplier [member] [ally] = per_cent_used_with / per_cent_used;
	}
}

void estimate_remaining_multipliers (float multiplier [Species::END] [Species::END], std::array<unsigned, Species::END> const & overall, std::array<unsigned, Species::END> const & unaccounted) {
	for (unsigned a = 0; a != Species::END; ++a) {
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

std::array<unsigned, Species::END> overall_stats () {
	return load_stats_from_file<unsigned> ("settings/usage.txt");
}

std::array<float, Species::END> lead_stats () {
	return load_stats_from_file<float> ("settings/lead.txt");
}

void team_stats (std::array<unsigned, Species::END> const & overall, unsigned const total, float multiplier [Species::END][Species::END]) {

	species_clause (multiplier);

	// There are 5 other Pokemon on a team for each individual Pokemon.
	// Therefore, if I've seen a Pokemon with n usages, there are 5 * n other
	// Pokemon on the team. Subtract the known number of usages from this
	// number until all known usages are gone. Then, assume the distribution of
	// Pokemon not on the team mate stats is equal to the relative overall
	// distribution and divide up all remaining usages proportionally.
	std::array<unsigned, Species::END> unaccounted;
	for (unsigned n = 0; n != Species::END; ++n)
		unaccounted [n] = overall [n] * 5;

	load_listed_multipliers (multiplier, overall, unaccounted, total);
	estimate_remaining_multipliers (multiplier, overall, unaccounted);
}

void detailed_stats (int detailed [][7]) {
	std::string const file_name = "settings/detailed.txt";
	std::ifstream file (file_name);
	std::string line;
	Species old_member = Species::END;
	bool ability = false;
	bool item = false;
	bool nature = false;
	unsigned move = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t const x = line.find ('\t');
		Species new_member = Pokemon::from_string (line.substr (0, x));
		if (new_member >= Species::END)
			throw InvalidSettingsFile (file_name, InvalidSettingsFile::invalid_data);
		if (old_member != new_member) {
			old_member = new_member;
			ability = false;
			item = false;
			nature = false;
			move = 0;
		}
		size_t const y = line.find ('\t', x + 1);
		size_t const z = line.find ('\t', y + 1);
		unsigned n = 7;
		int data;
		std::string sub = line.substr (x + 1, y - x - 1);
		if (sub == "Ability") {
			if (!ability ) {
				n = 0;
				ability = true;
				data = Ability::from_string (line.substr (y + 1, z - y - 1));
			}
		}
		else if (sub == "Item") {
			if (!item) {
				n = 1;
				item = true;
				data = Item::from_string (line.substr (y + 1, z - y - 1));
			}
		}
		else if (sub == "Nature") {
			if (!nature) {
				n = 2;
				nature = true;
				data = Nature::from_string (line.substr (y + 1, z - y - 1));
			}
		}
		else if (sub == "Move") {
			if (move < 4) {
				n = 3 + move;
				++move;
				data = Move::from_string (line.substr (y + 1, z - y - 1));
			}
		}
		if (n != 7)
			detailed [new_member] [n] = data;
	}
}

}	// namespace technicalmachine
