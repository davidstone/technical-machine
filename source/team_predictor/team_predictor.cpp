// Predict foe's team
// Copyright (C) 2014 David Stone
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

#include "detailed_stats.hpp"
#include "estimate.hpp"
#include "load_stats.hpp"
#include "multiplier.hpp"

#include "ev_optimizer/ev_optimizer.hpp"

#include "../ability.hpp"
#include "../team.hpp"

#include "../pokemon/pokemon.hpp"
#include "../pokemon/species.hpp"

#include "../move/moves_forward.hpp"

#include <bounded_integer/array.hpp>

#include <vector>

namespace technicalmachine {
namespace {

auto all_ones_array() {
	bounded::array<float, number_of_species> all_ones;
	all_ones.fill(1.0F);
	return all_ones;
}
void predict_pokemon(Team & team, Estimate estimate, Multiplier const & multiplier);
void predict_move (Pokemon & member, std::vector<Moves> const & detailed);

}	// namespace

Team predict_team (DetailedStats const & detailed, Team team, std::mt19937 & random_engine, bool using_lead) {
	auto const overall = overall_stats();
	constexpr unsigned total = 961058;	// Total number of teams
	Multiplier const multiplier(overall);
	
	auto const lead = using_lead ? lead_stats() : all_ones_array();
	
	Estimate estimate(overall, lead, total);
	estimate.update(multiplier, team);

	predict_pokemon (team, estimate, multiplier);
	for (auto & pokemon : team.all_pokemon()) {
		get_ability(pokemon).set_if_unknown(detailed.get<Ability::Abilities>(pokemon));
		get_item(pokemon).set_if_unknown(detailed.get<Item::Items>(pokemon));
		get_nature(pokemon).set_if_unknown(detailed.get<Nature::Natures>(pokemon));
		predict_move(pokemon, detailed.get<std::vector<Moves>>(pokemon));
		optimize_evs(pokemon, random_engine);
	}
	return team;
}

namespace {

void predict_pokemon(Team & team, Estimate estimate, Multiplier const & multiplier) {
	auto const index = team.pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		Species const name = estimate.most_likely();
		Level const level(100_bi);
		Gender const gender;
		team.add_pokemon(name, level, gender);
		if (team.number_of_seen_pokemon() == team.size())
			break;
		estimate.update(multiplier, name);
	}
	team.all_pokemon().set_index(index);
}

void predict_move (Pokemon & member, std::vector<Moves> const & detailed) {
	for (Moves const name : detailed) {
		bool const already_has_this_move = static_cast<bool>(member.move.index(name));
		if (already_has_this_move) {
			continue;
		}
		member.move.add(name);
		static constexpr unsigned max_regular_moves = 4;
		if (member.move.number_of_regular_moves() == max_regular_moves) {
			break;
		}
	}
}

}	// unnamed namespace
}	// namespace technicalmachine
