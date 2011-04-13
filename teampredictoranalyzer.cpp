// Check the accuracy of my team prediction function
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "pokemon.h"
#include "species.h"

using namespace technicalmachine;

// This was designed to test how accurate different versions of my team prediction function were to make sure my changes actually helped. It's useless now, but the code might be the base for a future such test.

void test_teams (const std::vector<double> &old_estimate, const std::vector<double> &new_estimate, unsigned old_correct[][6], unsigned new_correct [][6], const std::map<std::string, species> &species_map);

void combination (const std::vector<species> &team, const std::vector<species> &known, std::vector<double> estimate, double multiplier [][END_SPECIES], unsigned correct[][6]);

void tabulate (unsigned old_correct [][6], unsigned new_correct [][6]);

int main () {
	std::map <std::string, species> species_map;
	std::vector<double> overall;
	overall_stats (overall);
	double total = 961058;	// Total number of teams
	team_stats (overall, total);
	std::vector<double> lead;
	lead_stats (lead);
	
	std::vector<double> old_estimate;
	for (unsigned n = 0; n != END_SPECIES; ++n)
		old_estimate.push_back ((overall.at (n) / total) * lead.at (n));
	std::vector<double> new_estimate = old_estimate;

	unsigned old_correct [5][6] = {{ 0 }};
	unsigned new_correct [5][6] = {{ 0 }};
	test_teams (old_estimate, new_estimate, old_correct, new_correct, species_map);
	tabulate (old_correct, new_correct);
	return 0;
}

void test_teams (const std::vector<double> &old_estimate, const std::vector<double> &new_estimate, unsigned old_correct [][6], unsigned new_correct [][6], const std::map<std::string, species> &species_map) {
	// The "correct" matrices are [number of non-lead Pokemon already seen] [total Pokemon correctly predicted]
	std::ifstream file ("teams.txt");
	std::string line;
	getline (file, line);
	while (!file.eof()) {
		std::vector<species> team;
		unsigned begin = 0;
		unsigned end = 0;
		while (end != std::string::npos) {
			end = line.find (", ", end + 1);
			team.push_back (species_map.find (line.substr (begin, end - begin))->second);
			begin = end + 2;
		}
		if (team.size() != 6) {
			for (std::vector<species>::const_iterator it = team.begin(); it != team.end(); ++it)
				std::cout << pokemon_name [*it] << ", ";
		}
/*		for (std::vector<species>::const_iterator it = team.begin(); it != team.end(); ++it)
			std::cout << pokemon_name [*it] << ", ";
		std::cout << "is the correct team\n";
*/		std::vector<species> known;
		known.push_back (team.front());
		combination (team, known, old_estimate, old_multiplier, old_correct);
		combination (team, known, new_estimate, new_multiplier, new_correct);
		known.push_back (ABRA);
		for (int a = 1; a != 6; ++a) {
			known.at (1) = team.at (a);
			combination (team, known, old_estimate, old_multiplier, old_correct);
			combination (team, known, new_estimate, new_multiplier, new_correct);
		}
		known.push_back (ABRA);
		for (int a = 1; a != 5; ++a) {
			for (int b = a + 1; b != 6; ++b) {
				known.at (1) = team.at (a);
				known.at (2) = team.at (b);
				combination (team, known, old_estimate, old_multiplier, old_correct);
				combination (team, known, new_estimate, new_multiplier, new_correct);
			}
		}
		known.push_back (ABRA);
		for (int a = 1; a != 4; ++a) {
			for (int b = a + 1; b != 5; ++b) {
				for (int c = b + 1; c != 6; ++c) {
					known.at (1) = team.at (a);
					known.at (2) = team.at (b);
					known.at (3) = team.at (c);
					combination (team, known, old_estimate, old_multiplier, old_correct);
					combination (team, known, new_estimate, new_multiplier, new_correct);
				}
			}
		}
		known.push_back (ABRA);
		for (int a = 1; a != 3; ++a) {
			for (int b = a + 1; b != 4; ++b) {
				for (int c = b + 1; c != 5; ++c) {
					for (int d = c + 1; d != 6; ++d) {
						known.at (1) = team.at (a);
						known.at (2) = team.at (b);
						known.at (3) = team.at (c);
						known.at (4) = team.at (d);
						combination (team, known, old_estimate, old_multiplier, old_correct);
						combination (team, known, new_estimate, new_multiplier, new_correct);
					}
				}
			}
		}
		getline (file, line);
	}
}

void combination (const std::vector<species> &team, const std::vector<species> &known, std::vector<double> estimate, double multiplier [][END_SPECIES], unsigned correct[][6]) {
	for (std::vector<species>::const_iterator it = known.begin(); it != known.end(); ++it) {
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [*it] [n];
	}
	std::vector <species> output = aggressive (known, estimate, multiplier);
	unsigned x = 0;		// Number of Pokemon that match the real team and the predicted team
	for (std::vector<species>::const_iterator it = team.begin(); it != team.end(); ++it) {
		for (std::vector<species>::const_iterator guess = output.begin(); guess != output.end(); ++guess) {
			if (*it == *guess) {
				++x;
				break;
			}
		}
	}
	for (unsigned n = 0; n != x; ++n)
		++correct [known.size() - 1] [n];
/*	for (std::vector<species>::const_iterator it = output.begin(); it != output.end(); ++it)
		std::cout << pokemon_name [*it] << ", ";
	std::cout << "\n";
*/}

void tabulate (unsigned old_correct [][6], unsigned new_correct [][6]) {
	for (unsigned n = 0; n != 5; ++n) {
		std::cout << "When " << n + 1 << " Pokemon ";
		if (n == 0)
			std::cout << "has";
		else
			std::cout << "have";
		std::cout << " been seen, the prediction is this good:\n";
		for (unsigned m = n; m != 6; ++m) {
			if (old_correct [n][m] != 0 or new_correct [n][m] != 0) {
				if (old_correct [n][m] == new_correct [n][m])
					std::cout << "Both identified at least " << m + 1 << " Pokemon " << old_correct [n][m] << " times.\n";
				else {
					std::cout << "Old identified at least " << m + 1 << " Pokemon " << old_correct [n][m] << " times.\n";
					std::cout << "New identified at least " << m + 1 << " Pokemon " << new_correct [n][m] << " times.\n";
				}
			}
		}
		double old_total = 0.0;
		double new_total = 0.0;
		for (unsigned m = n + 1; m != 6; ++m) {
			old_total += (old_correct [n][m]);
			new_total += (new_correct [n][m]);
		}
		std::cout << "Old version is correct " << 100.0 * old_total / static_cast<double> (old_correct [n][n] * (5 - n)) << "% of the time\n";
		std::cout << "New version is correct " << 100.0 * new_total / static_cast<double> (new_correct [n][n] * (5 - n)) << "% of the time\n";
	}
}
