// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/estimate.hpp>
#include <tm/team_predictor/ev_optimizer/ev_optimizer.hpp>
#include <tm/team_predictor/stats_user.hpp>

#include <tm/seen_team.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/size.hpp>

#include <random>

namespace technicalmachine {

struct UsageStats;

void predict_pokemon(StatsUser & stats_user, any_seen_team auto & team, std::mt19937 & random_engine) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		auto const species = stats_user.species(random_engine);
		auto const level = Level(100_bi);
		team.add_pokemon({species, level, Gender::genderless});
		if (team.number_of_seen_pokemon() == team.size()) {
			break;
		}
		stats_user.update(species);
	}
	team.all_pokemon().set_index(index);
}

template<any_seen_pokemon PokemonType>
void predict_moves(PokemonType & pokemon, containers::static_vector<Moves, max_moves_per_pokemon> const detailed) {
	for (Moves const move_name : detailed) {
		if (containers::size(pokemon.regular_moves()) == max_moves_per_pokemon) {
			break;
		}
		if (containers::any_equal(pokemon.regular_moves(), move_name)) {
			continue;
		}
		pokemon.add_move(Move(generation_from<PokemonType>, move_name));
	}
}

void optimize_pokemon_evs(any_seen_pokemon auto & pokemon, std::mt19937 & random_engine) {
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

template<Generation generation>
auto predict_team_impl(StatsUser stats_user, std::mt19937 & random_engine, SeenTeam<generation> team) -> Team<generation> {
	update_estimate(stats_user, team);

	predict_pokemon(stats_user, team, random_engine);
	for (auto & pokemon : team.all_pokemon()) {
		auto const species = pokemon.species();
		if (!pokemon.ability_is_known()) {
			pokemon.set_initial_ability(stats_user.ability(species));
		}
		if (!pokemon.item_is_known()) {
			pokemon.set_item(stats_user.item(species));
		}
		predict_moves(pokemon, stats_user.moves(species));
		optimize_pokemon_evs(pokemon, random_engine);
	}
	// TODO: This isn't right
	team.pokemon().set_ability_to_base_ability();
	return Team<generation>(team);
}

template<Generation generation>
auto most_likely_team(UsageStats const & usage_stats, std::mt19937 & random_engine, SeenTeam<generation> team) -> Team<generation> {
	return predict_team_impl(StatsUser(usage_stats, true), random_engine, team);
}

template<Generation generation>
auto random_team(UsageStats const & usage_stats, std::mt19937 & random_engine, SeenTeam<generation> team = SeenTeam<generation>(max_pokemon_per_team)) -> Team<generation> {
	return predict_team_impl(StatsUser(usage_stats, false), random_engine, team);
}

} // namespace technicalmachine
