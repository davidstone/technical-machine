// Predict foe's team
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <boost/lexical_cast.hpp>
#include <fstream>
#include <map>
#include <vector>
#include "pokemon.h"
#include "species.h"
#include "team.h"
#include "teampredictor.h"

#include <iostream>

namespace technicalmachine {

void overall_stats (std::vector<double> &overall) {
	std::ifstream file ("usage.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line))
		overall.push_back (boost::lexical_cast<double> (line));
	file.close ();
}

void team_stats (const std::vector<double> &overall, double total, double multiplier [END_SPECIES][END_SPECIES]) {
	for (unsigned n = 0; n != END_SPECIES; ++n) {
		for (unsigned m = 0; m != END_SPECIES; ++m) {
			if (((n == DEOXYS_A or n == DEOXYS_D or n == DEOXYS_M or n == DEOXYS_S) and (m == DEOXYS_A or m == DEOXYS_D or m == DEOXYS_M or m == DEOXYS_S))
			 or ((n == GIRATINA_A or n == GIRATINA_O) and (m == GIRATINA_A or m == GIRATINA_O))
			 or ((n == ROTOM_C or n == ROTOM_F or n == ROTOM_H or n == ROTOM_S or n == ROTOM_W) and (m == ROTOM_C or m == ROTOM_F or m == ROTOM_H or m == ROTOM_S or m == ROTOM_W))
			 or ((n == SHAYMIN_L or n == SHAYMIN_S) and (m == SHAYMIN_L or m == SHAYMIN_S))
			 or ((n == WORMADAM_P or n == WORMADAM_S or n == WORMADAM_T) and (m == WORMADAM_P or m == WORMADAM_S or m == WORMADAM_T)))
				multiplier [n][m] = 0;	// Species clause
			else
				multiplier [n][m] = -1;		// Replaced with other value later
		}
		multiplier [n][n] = 0;			// Species clause
	}

	// There are 5 other Pokemon on a team for each individual Pokemon. Therefore, if I've seen a Pokemon with n usages, there are 5 * n other Pokemon on the team. Subtract the known number of usages from this number until all known usages are gone. Then, assume the distribution of Pokemon not on the team mate stats is equal to the relative overall distribution and divide up all remaining usages proportionally.
	double unaccounted [END_SPECIES];
	for (unsigned n = 0; n != END_SPECIES; ++n)
		unaccounted [n] = overall.at (n) * 5;

	std::ifstream file ("teammate.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t x = line.find ('\t');
		int member = boost::lexical_cast<int> (line.substr (0, x));
		size_t y = line.find ('\t', x + 1);
		int ally = boost::lexical_cast<int> (line.substr (x + 1, y - x - 1));
		// Subtract known usages
		unaccounted [member] -= boost::lexical_cast<double> (line.substr (y + 1));
		// multiplier = % used with this Pokemon / % used overall
		multiplier [member][ally] = (boost::lexical_cast<double> (line.substr (y + 1)) / overall.at (member)) / (overall.at (ally) / total);
	}
	for (unsigned n = 0; n != END_SPECIES; ++n) {
		if (overall.at (n) == 0) {
			for (unsigned m = 0; m != END_SPECIES; ++m)
				// 1 is superior to 0 because if they use an unused Pokemon, this will have no effect instead of making everything equally 0
				multiplier [n][m] = 1;
		}
		else {
			for (unsigned m = 0; m != END_SPECIES; ++m) {
				if (multiplier [n][m] == -1) {
					// New method takes the total number of remaining Pokemon not accounted for and assumes they're evenly distributed (based on overall stats) among all Pokemon not on the list. This is the same as giving all those Pokemon the same multiplier. Reality probably has very low usage Pokemon more evenly distributed and high usage Pokemon that don't appear with a much lower multiplier.
					multiplier [n][m] = unaccounted [n] / (overall.at (n) * 5);
				}
			}
		}
	}
	file.close ();
}

void lead_stats (std::vector<double> &lead) {		// Multiplier for Pokemon after you've seen the lead
	std::ifstream file ("lead.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line))
		lead.push_back (boost::lexical_cast<double> (line));
}

void detailed_stats (const Map &map, int detailed [][7]) {
	std::ifstream file ("detailed.txt");
	std::string line;
	species old_member = END_SPECIES;
	bool ability = false;
	bool item = false;
	bool nature = false;
	unsigned move = 0;
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t x = line.find ('\t');
		species new_member = map.specie.find (line.substr (0, x))->second;
		if (old_member != new_member) {
			old_member = new_member;
			ability = false;
			item = false;
			nature = false;
			move = 0;
		}
		size_t y = line.find ('\t', x + 1);
		size_t z = line.find ('\t', y + 1);
		unsigned n = 7;
		int data;
		std::string sub = line.substr (x + 1, y - x - 1);
		if (sub == "Ability") {
			if (!ability ) {
				n = 0;
				ability = true;
				data = map.ability.find (line.substr (y + 1, z - y - 1))->second;
			}
		}
		else if (sub == "Item") {
			if (!item) {
				n = 1;
				item = true;
				data = map.item.find (line.substr (y + 1, z - y - 1))->second;
			}
		}
		else if (sub == "Nature") {
			if (!nature) {
				n = 2;
				nature = true;
				data = map.nature.find (line.substr (y + 1, z - y - 1))->second;
			}
		}
		else if (sub == "Move") {
			if (move < 4) {
				n = 3 + move;
				++move;
				data = map.move.find (line.substr (y + 1, z - y - 1))->second;
			}
		}
		if (n != 7)
			detailed [new_member] [n] = data;
	}
}

void predict (int detailed [][7], Team &team) {
	std::vector<double> overall;
	overall_stats (overall);
	double total = 961058;	// Total number of teams
	double multiplier [END_SPECIES][END_SPECIES];
	team_stats (overall, total, multiplier);
	std::vector<double> lead;
	lead_stats (lead);
	
	std::vector<double> estimate;
	for (unsigned n = 0; n != END_SPECIES; ++n)
		estimate.push_back ((overall.at (n) / total) * lead.at (n));

	for (std::vector<Pokemon>::const_iterator it = team.active.member.begin(); it != team.active.member.end(); ++it) {
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [it->name] [n];
	}
	predict_pokemon (team, estimate, detailed, multiplier);
}

void predict_pokemon (Team &team, std::vector<double> estimate, int detailed [][7], double multiplier [END_SPECIES][END_SPECIES]) {
	while (team.active.member.size() < 6) {
		double top = 0.0;
		species name;
		for (int n = 0; n != END_SPECIES; ++n) {
			if (top < estimate.at (n)) {
				top = estimate.at (n);
				name = static_cast<species> (n);
			}
		}
		Pokemon member (name);
		team.active.member.push_back (member);
		if (team.active.member.size() == 6)
			break;
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [team.active.member.back().name] [n];
	}
	for (std::vector<Pokemon>::iterator it = team.active.member.begin(); it != team.active.member.end(); ++it) {
		it->level = 100;
		it->ability = static_cast<abilities> (detailed [it->name] [0]);
		it->item = static_cast<items> (detailed [it->name] [1]);
		it->nature = static_cast<natures> (detailed [it->name] [2]);
		it->gender = GENDERLESS;
		predict_move (*it, detailed);
		loadpokemon (team, *it);
	}
}

void predict_move (Pokemon &member, int detailed [][7]) {
	for (unsigned n = 3;
	 member.moveset.size() < 4
	 and detailed [member.name] [n] != END_MOVE;
	 ++n) {
		bool found = false;
		for (std::vector<Move>::const_iterator it = member.moveset.begin(); it != member.moveset.end(); ++it) {
			if (it->name == static_cast<moves_list> (detailed [member.name] [n])) {
				found = true;
				break;
			}
		}
		if (!found) {
			Move move (static_cast<moves_list> (detailed [member.name] [n]), 3);
			member.moveset.insert (member.moveset.begin(), move);
		}
	}
}

}
