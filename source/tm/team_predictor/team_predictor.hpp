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

#pragma once

#include <tm/team_predictor/estimate.hpp>
#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>
#include <tm/team_predictor/lead_stats.hpp>
#include <tm/team_predictor/random_team.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/team.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/size.hpp>

#include <random>

namespace technicalmachine {

enum class Generation : std::uint8_t;
struct UsageStats;

template<Generation generation>
void predict_pokemon(Team<generation> & team, Estimate estimate, UsageStats const & usage_stats, std::mt19937 & random_engine, bool const use_most_likely) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		Species const name = use_most_likely ? estimate.most_likely() : estimate.random(random_engine);
		Level const level(100_bi);
		team.add_pokemon(Pokemon<generation>(name, level, Gender::genderless));
		if (team.number_of_seen_pokemon() == team.size())
			break;
		estimate.update(usage_stats, name);
	}
	team.all_pokemon().set_index(index);
}

inline void predict_move(RegularMoves & regular, Generation const generation, containers::static_vector<Moves, max_moves_per_pokemon.value()> const detailed) {
	for (Moves const move_name : detailed) {
		if (containers::size(regular) == max_moves_per_pokemon) {
			break;
		}
		if (containers::any_equal(regular, move_name)) {
			continue;
		}
		regular.push_back(Move(generation, move_name));
	}
}

template<Generation generation>
auto predict_team_impl(UsageStats const & usage_stats, LeadStats const lead_stats, Team<generation> team, std::mt19937 & random_engine, bool const use_most_likely) {
	auto estimate = Estimate(usage_stats, lead_stats);
	update_estimate(estimate, usage_stats, team);

	predict_pokemon(team, estimate, usage_stats, random_engine, use_most_likely);
	for (auto & pokemon : team.all_pokemon()) {
		auto const species = pokemon.species();
		auto const & detailed = usage_stats.get(species);
		if (!pokemon.ability_is_known()) {
			pokemon.set_initial_ability(detailed.ability);
		}
		if (!pokemon.item_is_known()) {
			pokemon.set_item(detailed.item);
		}
		if (!pokemon.nature_is_known()) {
			pokemon.set_nature(detailed.stats.nature);
		}
		predict_move(pokemon.regular_moves(), generation, detailed.moves);
		optimize_evs(pokemon, random_engine);
	}
	// TODO: This isn't right
	team.pokemon().set_ability_to_base_ability();
	return team;
}

template<Generation generation>
auto predict_team(UsageStats const & usage_stats, LeadStats const lead_stats, Team<generation> team, std::mt19937 & random_engine) {
	return predict_team_impl(usage_stats, lead_stats, team, random_engine, true);
}

template<Generation generation>
auto generate_team(UsageStats const & usage_stats, LeadStats lead_stats, std::mt19937 & random_engine) {
	return predict_team_impl(usage_stats, lead_stats, Team<generation>(max_pokemon_per_team, true), random_engine, false);
}


}	// namespace technicalmachine
