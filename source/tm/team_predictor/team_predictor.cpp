// Predict foe's team
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

#include <tm/team_predictor/team_predictor.hpp>

#include <tm/team_predictor/estimate.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>

#include <tm/ability.hpp>
#include <tm/team.hpp>

#include <tm/pokemon/pokemon.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/array/make_array.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace {

void predict_pokemon(Generation const generation, Team & team, Estimate estimate, UsageStats const & usage_stats, std::mt19937 & random_engine, bool const use_most_likely) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		Species const name = use_most_likely ? estimate.most_likely() : estimate.random(random_engine);
		Level const level(100_bi);
		team.add_pokemon(generation, name, level, Gender::genderless);
		if (team.number_of_seen_pokemon() == team.size())
			break;
		estimate.update(usage_stats, name);
	}
	team.all_pokemon().set_index(index);
}

void predict_move(MoveContainer & moves, Generation const generation, containers::static_vector<Moves, max_moves_per_pokemon.value()> const detailed) {
	for (Moves const move : detailed) {
		auto const regular = moves.regular();
		if (size(regular) == max_moves_per_pokemon) {
			break;
		}
		if (containers::any_equal(regular, move)) {
			continue;
		}
		moves.add(Move(generation, move));
	}
}

auto predict_team_impl(Generation const generation, UsageStats const & usage_stats, LeadStats const lead_stats, Team team, std::mt19937 & random_engine, bool const use_most_likely) -> Team {
	auto estimate = Estimate(usage_stats, lead_stats);
	update_estimate(estimate, usage_stats, team);

	predict_pokemon(generation, team, estimate, usage_stats, random_engine, use_most_likely);
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
		predict_move(pokemon.all_moves(), generation, detailed.moves);
		optimize_evs(generation, pokemon, random_engine);
	}
	// TODO: This isn't right
	team.pokemon().set_ability_to_base_ability();
	return team;
}

}	// namespace

auto predict_team(Generation const generation, UsageStats const & usage_stats, LeadStats const lead_stats, Team team, std::mt19937 & random_engine) -> Team {
	return predict_team_impl(generation, usage_stats, lead_stats, team, random_engine, true);
}

auto generate_team(Generation generation, UsageStats const & usage_stats, LeadStats lead_stats, std::mt19937 & random_engine) -> Team {
	return predict_team_impl(generation, usage_stats, lead_stats, Team(max_pokemon_per_team, true), random_engine, false);
}

} // namespace technicalmachine
