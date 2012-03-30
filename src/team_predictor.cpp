// Predict foe's team
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

#include "team_predictor.hpp"

#include <vector>

#include "ability.hpp"
#include "load_stats.hpp"
#include "pokemon.hpp"
#include "species.hpp"
#include "team.hpp"

namespace technicalmachine {
namespace {

void predict_pokemon (Team & team, std::vector<float> estimate, float multiplier [Species::END][Species::END]);
Species get_most_likely_pokemon (std::vector <float> const & estimate);
void predict_move (Pokemon & member, int detailed [][7], unsigned size);

}	// unnamed namespace

Team predict_team (int detailed [][7], Team team, unsigned size, bool using_lead) {
	std::vector<unsigned> const overall = overall_stats ();
	constexpr unsigned total = 961058;	// Total number of teams
	float multiplier [Species::END][Species::END];
	team_stats (overall, total, multiplier);
	std::vector<float> const lead = using_lead ? lead_stats () : std::vector <float> (Species::END, 1);
	
	std::vector<float> estimate;
	estimate.reserve (Species::END);
	for (unsigned n = 0; n != Species::END; ++n)
		estimate.push_back (lead [n] * overall [n] / total);

	for (Pokemon const & pokemon : team.pokemon.set) {
		for (unsigned n = 0; n != Species::END; ++n)
			estimate [n] *= multiplier [pokemon.name] [n];
	}
	predict_pokemon (team, estimate, multiplier);
	for (Pokemon & pokemon : team.pokemon.set) {
		pokemon.ability.set_if_unknown (static_cast <Ability::Abilities> (detailed [pokemon.name] [0]));
		if (!pokemon.item.is_set ())
			pokemon.item.name = static_cast <Item::Items> (detailed [pokemon.name] [1]);
		if (!pokemon.nature.is_set ())
			pokemon.nature.name = static_cast <Nature::Natures> (detailed [pokemon.name] [2]);
		predict_move (pokemon, detailed, size);
	}
	return team;
}

namespace {

void predict_pokemon (Team & team, std::vector<float> estimate, float multiplier [Species::END] [Species::END]) {
	while (team.pokemon.set.size() < team.size) {
		Species const name = get_most_likely_pokemon (estimate);
		Pokemon member (name, team.size);
		member.load ();
		team.pokemon.set.push_back (member);
		if (team.pokemon.set.size() == team.size)
			break;
		for (unsigned n = 0; n != Species::END; ++n)
			estimate [n] *= multiplier [team.pokemon.set.back().name] [n];
	}
}

Species get_most_likely_pokemon (std::vector <float> const & estimate) {
	Species name = Species::END;
	float top = -1.0;
	for (unsigned n = 0; n != Species::END; ++n) {
		if (estimate [n] > top) {
			top = estimate [n];
			name = static_cast<Species> (n);
		}
	}
	return name;
}

void predict_move (Pokemon & member, int detailed [][7], unsigned size) {
	// Pokemon I've already seen will have their moveset filled out with
	// Struggle and Switch# for each Pokemon still alive in their team. This
	// makes sure that those Pokemon get all of their moves predicted.
	unsigned n = 0;
	while (member.move.set [n].name != Move::STRUGGLE)
		++n;
	unsigned max_moves = 4 + member.move.set.size() - n;
	for (unsigned m = 3; member.move.set.size() < max_moves and detailed [member.name] [m] != Move::END; ++m) {
		bool found = false;
		for (std::vector<Move>::const_iterator it = member.move.set.begin(); it->name != Move::STRUGGLE; ++it) {
			if (it->name == static_cast<Move::Moves> (detailed [member.name] [m])) {
				found = true;
				break;
			}
		}
		if (!found) {
			Move move (static_cast<Move::Moves> (detailed [member.name] [m]), 3, size);
			// I use n here so that already seen moves (guaranteed to exist)
			// are listed earlier in the move set. I increment n so that moves
			// are listed in the order of their probability for predicted moves
			// as well. This also has the advantage of requiring fewer shifts
			// of my vector.
			member.move.set.insert (member.move.set.begin() + n, move);
			++n;
		}
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
