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

#include <vector>
#include "load_stats.h"
#include "pokemon.h"
#include "team.h"
#include "teampredictor.h"

#include <iostream>

namespace technicalmachine {

void predict (int detailed [][7], Team &team) {
	std::cout << "======================\nPredicting...\n";

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

	for (std::vector<Pokemon>::const_iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [pokemon->name] [n];
	}
	predict_pokemon (team, estimate, detailed, multiplier);

	std::string out;
	output (team, out);
	std::cout << out;
}

void predict_pokemon (Team &team, std::vector<double> estimate, int detailed [][7], double multiplier [END_SPECIES][END_SPECIES]) {
	while (team.pokemon.set.size() < team.size) {
		double top = 0.0;
		species name;
		for (int n = 0; n != END_SPECIES; ++n) {
			if (top < estimate.at (n)) {
				top = estimate.at (n);
				name = static_cast<species> (n);
			}
		}
		Pokemon member (name);
		team.pokemon.set.push_back (member);
		if (team.pokemon.set.size() == 6)
			break;
		for (unsigned n = 0; n != END_SPECIES; ++n)
			estimate.at (n) *= multiplier [team.pokemon.set.back().name] [n];
	}
	for (std::vector<Pokemon>::iterator it = team.pokemon.set.begin(); it != team.pokemon.set.end(); ++it) {
		it->level = 100;
		if (it->ability == END_ABILITY)
			it->ability = static_cast<abilities> (detailed [it->name] [0]);
		if (it->item == END_ITEM)
			it->item = static_cast<items> (detailed [it->name] [1]);
		it->nature = static_cast<natures> (detailed [it->name] [2]);
		it->gender = GENDERLESS;
		predict_move (*it, detailed);
		loadpokemon (team, *it);
	}
}

void predict_move (Pokemon &member, int detailed [][7]) {
	// Pokemon I've already seen will have their moveset filled out with Struggle and Switch# for each Pokemon still alive in their team. This makes sure that those Pokemon get all of their moves predicted.
	bool seen = false;
	unsigned n = 0;
	for ( ; n != member.move.set.size(); ++n) {
		if (member.move.set [n].name == STRUGGLE) {
			seen = true;
			break;
		}
	}
	unsigned max_moves = 4;
	if (seen)
		max_moves += member.move.set.size() - n;
	for (unsigned m = 3; member.move.set.size() < max_moves and detailed [member.name] [m] != END_MOVE; ++m) {
		bool found = false;
		for (std::vector<Move>::const_iterator it = member.move.set.begin(); it != member.move.set.end(); ++it) {
			if (it->name == static_cast<moves_list> (detailed [member.name] [m])) {
				found = true;
				break;
			}
		}
		if (!found) {
			Move move (static_cast<moves_list> (detailed [member.name] [m]), 3);
			// I use n here so that already seen moves (guaranteed to exist) are listed earlier in the move set. I increment n so that moves are listed in the order of their probability for predicted moves as well. This also has the advantage of requiring fewer shifts of my vector.
			member.move.set.insert (member.move.set.begin() + n, move);
			++n;
		}
	}
}

}
