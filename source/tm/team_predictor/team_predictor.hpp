// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

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

struct UsageStats;

template<any_team TeamType>
void predict_pokemon(TeamType & team, Estimate estimate, UsageStats const & usage_stats, std::mt19937 & random_engine, bool const use_most_likely) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		auto const species = use_most_likely ? estimate.most_likely() : estimate.random(random_engine);
		auto const level = Level(100_bi);
		team.add_pokemon({species, level, Gender::genderless});
		if (team.number_of_seen_pokemon() == team.size()) {
			break;
		}
		estimate.update(usage_stats, species);
	}
	team.all_pokemon().set_index(index);
}

template<Generation generation>
void predict_moves(Pokemon<generation> & pokemon, containers::static_vector<Moves, max_moves_per_pokemon.value()> const detailed) {
	for (Moves const move_name : detailed) {
		if (containers::size(pokemon.regular_moves()) == max_moves_per_pokemon) {
			break;
		}
		if (containers::any_equal(pokemon.regular_moves(), move_name)) {
			continue;
		}
		pokemon.add_move(Move(generation, move_name));
	}
}

template<Generation generation>
void optimize_pokemon_evs(Pokemon<generation> & pokemon, std::mt19937 & random_engine) {
	auto const species = pokemon.species();
	auto const level = pokemon.level();
	auto const include_attack = has_physical_move(pokemon);
	auto const include_special_attack = has_special_move(pokemon);
	auto const optimized = optimize_evs(
		calculate_ivs_and_evs(pokemon),
		species,
		level,
		pokemon.hidden_power(),
		include_attack,
		include_special_attack,
		random_engine
	);
	pokemon.set_ivs_and_evs(optimized);
}

template<any_team TeamType>
auto predict_team_impl(UsageStats const & usage_stats, LeadStats const lead_stats, TeamType team, std::mt19937 & random_engine, bool const use_most_likely) -> TeamType {
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
		predict_moves(pokemon, detailed.moves);
		optimize_pokemon_evs(pokemon, random_engine);
	}
	// TODO: This isn't right
	team.pokemon().set_ability_to_base_ability();
	return team;
}

template<any_team TeamType>
auto predict_team(UsageStats const & usage_stats, LeadStats const lead_stats, TeamType team, std::mt19937 & random_engine) -> TeamType {
	return predict_team_impl(usage_stats, lead_stats, team, random_engine, true);
}

template<Generation generation>
auto generate_team(UsageStats const & usage_stats, LeadStats lead_stats, std::mt19937 & random_engine) -> Team<generation> {
	return predict_team_impl(usage_stats, lead_stats, Team<generation>(max_pokemon_per_team, true), random_engine, false);
}

} // namespace technicalmachine
