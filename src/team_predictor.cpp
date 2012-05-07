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

#include <array>
#include <vector>

#include "ability.hpp"
#include "detailed_stats.hpp"
#include "load_stats.hpp"
#include "pokemon.hpp"
#include "species.hpp"
#include "team.hpp"

namespace technicalmachine {
namespace {

template<typename T>
std::array<T, Species::END> all_ones_array () {
	std::array<T, Species::END> all_ones;
	all_ones.fill (1);
	return all_ones;
}
void predict_pokemon (Team & team, std::array<float, Species::END> estimate, float multiplier [Species::END][Species::END]);
Species get_most_likely_pokemon (std::array<float, Species::END> const & estimate);
void predict_move (Pokemon & member, std::vector<Move::Moves> const & detailed, unsigned size);

}	// unnamed namespace

Team predict_team (DetailedStats const & detailed, Team team, unsigned size, bool using_lead) {
	std::array<unsigned, Species::END> const overall = overall_stats ();
	constexpr unsigned total = 961058;	// Total number of teams
	float multiplier [Species::END][Species::END];
	team_stats (overall, total, multiplier);
	std::array<float, Species::END> const lead = using_lead ? lead_stats () : all_ones_array<float>();
	
	std::array<float, Species::END> estimate;
	for (unsigned n = 0; n != Species::END; ++n)
		estimate [n] = lead [n] * overall [n] / total;

	team.pokemon.for_each([& estimate, & multiplier](Pokemon const & pokemon) {
		for (unsigned n = 0; n != Species::END; ++n)
			estimate [n] *= multiplier [pokemon.name] [n];
	});
	predict_pokemon (team, estimate, multiplier);
	team.pokemon.for_each([& detailed, size](Pokemon & pokemon) {
		pokemon.ability.set_if_unknown (static_cast <Ability::Abilities> (detailed.ability [pokemon.name]));
		pokemon.item.set_if_unknown (static_cast <Item::Items> (detailed.item [pokemon.name]));
		pokemon.nature.set_if_unknown (static_cast <Nature::Natures> (detailed.nature [pokemon.name]));
		predict_move (pokemon, detailed.move[pokemon.name], size);
	});
	return team;
}

namespace {

void predict_pokemon (Team & team, std::array<float, Species::END> estimate, float multiplier [Species::END] [Species::END]) {
	while (team.pokemon.size() < team.pokemon.real_size()) {
		Species const name = get_most_likely_pokemon (estimate);
		team.add_pokemon (name);
		if (team.pokemon.size() == team.pokemon.real_size())
			break;
		for (unsigned n = 0; n != Species::END; ++n)
			estimate [n] *= multiplier [team.pokemon(team.pokemon.size() - 1).name] [n];
	}
}

Species get_most_likely_pokemon (std::array<float, Species::END> const & estimate) {
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

void predict_move (Pokemon & member, std::vector<Move::Moves> const & detailed, unsigned size) {
	for (Move::Moves const name : detailed) {
		Move const * const move_ptr = member.move.find_if([name](Move const & move) {
			return move.name == name;
		});
		if (move_ptr == nullptr) {
			member.move.add (Move (name, 3, size));
			if (member.move.number_of_regular_moves() == Move::max_regular_moves())
				break;
		}
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
