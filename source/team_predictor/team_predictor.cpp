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

#include "detailed_stats.hpp"
#include "load_stats.hpp"

#include "../ability.hpp"
#include "../team.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../move/move.hpp"

namespace technicalmachine {
enum class Moves : uint16_t;
namespace {

template<typename T>
std::array<T, max_species> all_ones_array () {
	std::array<T, max_species> all_ones;
	all_ones.fill (1);
	return all_ones;
}
void predict_pokemon (Team & team, std::array<float, max_species> estimate, float multiplier [max_species][max_species]);
Species get_most_likely_pokemon (std::array<float, max_species> const & estimate);
void predict_move (Pokemon & member, std::vector<Moves> const & detailed, unsigned size);

}	// unnamed namespace

Team predict_team (DetailedStats const & detailed, Team team, unsigned size, bool using_lead) {
	std::array<unsigned, max_species> const overall = overall_stats ();
	constexpr unsigned total = 961058;	// Total number of teams
	float multiplier [max_species][max_species];
	team_stats (overall, total, multiplier);
	std::array<float, max_species> const lead = using_lead ? lead_stats () : all_ones_array<float>();
	
	std::array<float, max_species> estimate;
	for (unsigned n = 0; n != max_species; ++n)
		estimate [n] = lead [n] * overall [n] / total;

	team.pokemon.for_each([& estimate, & multiplier](Pokemon const & pokemon) {
		for (unsigned n = 0; n != max_species; ++n)
			estimate [n] *= multiplier[static_cast<size_t>(pokemon.name)] [n];
	});
	predict_pokemon (team, estimate, multiplier);
	team.pokemon.for_each([& detailed, size](Pokemon & pokemon) {
		pokemon.ability.set_if_unknown (static_cast <Ability::Abilities> (detailed.ability[static_cast<size_t>(pokemon.name)]));
		pokemon.item.set_if_unknown (static_cast <Item::Items> (detailed.item[static_cast<size_t>(pokemon.name)]));
		pokemon.nature.set_if_unknown (static_cast <Nature::Natures> (detailed.nature[static_cast<size_t>(pokemon.name)]));
		predict_move (pokemon, detailed.move[static_cast<size_t>(pokemon.name)], size);
	});
	return team;
}

namespace {

void predict_pokemon (Team & team, std::array<float, max_species> estimate, float multiplier [max_species] [max_species]) {
	auto const index = team.pokemon.index();
	while (team.pokemon.size() < team.pokemon.real_size()) {
		Species const name = get_most_likely_pokemon (estimate);
		constexpr unsigned level = 100;
		Gender const gender;
		team.add_pokemon(name, level, gender);
		if (team.pokemon.size() == team.pokemon.real_size())
			break;
		for (unsigned n = 0; n != max_species; ++n)
			estimate [n] *= multiplier[static_cast<size_t>(team.pokemon(team.pokemon.size() - 1).name)][n];
	}
	team.pokemon.set_index(index);
}

Species get_most_likely_pokemon (std::array<float, max_species> const & estimate) {
	Species name = Species::END;
	float top = -1.0;
	for (unsigned n = 0; n != max_species; ++n) {
		if (estimate [n] > top) {
			top = estimate [n];
			name = static_cast<Species> (n);
		}
	}
	return name;
}

void predict_move (Pokemon & member, std::vector<Moves> const & detailed, unsigned size) {
	for (Moves const name : detailed) {
		if (!member.move.exists(name)) {
			member.move.add(name, 3, size);
			if (member.move.number_of_regular_moves() == Move::max_regular_moves())
				break;
		}
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
