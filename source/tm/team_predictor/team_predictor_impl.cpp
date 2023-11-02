// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

module tm.team_predictor.team_predictor;

import tm.move.max_moves_per_pokemon;
import tm.move.move;

import tm.pokemon.any_pokemon;
import tm.pokemon.level;
import tm.pokemon.has_physical_or_special_move;
import tm.pokemon.max_pokemon_per_team;

import tm.stat.calculate_ivs_and_evs;

import tm.team_predictor.estimate;
import tm.team_predictor.ev_optimizer.ev_optimizer;
import tm.team_predictor.stats_user;
import tm.team_predictor.usage_stats;

import tm.any_team;
import tm.gender;
import tm.generation;
import tm.team;

import bounded;
import containers;
import std_module;

namespace technicalmachine {
using namespace bounded::literal;

void predict_pokemon(StatsUser & stats_user, any_seen_team auto & team) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		auto const species = stats_user.species();
		if (!species) {
			throw std::runtime_error("Usage stats say there are not enough Pokemon to make a team");
		}
		stats_user.update(*species);
		auto const level = Level(100_bi);
		team.add_pokemon({*species, level, Gender::genderless});
		if (team.number_of_seen_pokemon() == team.size()) {
			break;
		}
	}
	team.all_pokemon().set_index(index);
}

template<any_seen_pokemon PokemonType>
void predict_moves(StatsUser & stats_user, PokemonType & pokemon) {
	auto const species = pokemon.species();
	while (containers::size(pokemon.regular_moves()) != max_moves_per_pokemon) {
		auto const move_name = stats_user.move(species);
		if (!move_name) {
			break;
		}
		pokemon.add_move(Move(generation_from<PokemonType>, *move_name));
		stats_user.update(species, *move_name);
	}
}

void predict_item(StatsUser & stats_user, any_seen_pokemon auto & pokemon) {
	if (pokemon.item_is_known()) {
		return;
	}
	auto const species = pokemon.species();
	auto const item = stats_user.item(species);
	if (!item) {
		return;
	}
	pokemon.set_item(*item);
	stats_user.update(species, *item);
}

void predict_ability(StatsUser & stats_user, any_seen_pokemon auto & pokemon) {
	if (pokemon.ability_is_known()) {
		return;
	}
	auto const species = pokemon.species();
	auto const ability = stats_user.ability(species);
	if (!ability) {
		return;
	}
	pokemon.set_initial_ability(*ability);
	stats_user.update(species, *ability);
}

void optimize_pokemon_evs(any_seen_pokemon auto & pokemon) {
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
		include_special_attack
	);
	pokemon.set_ivs_and_evs(optimized);
}

template<Generation generation>
auto predict_team_impl(StatsUser stats_user, SeenTeam<generation> team) -> Team<generation> {
	update_estimate(stats_user, team);

	predict_pokemon(stats_user, team);
	for (auto & pokemon : team.all_pokemon()) {
		predict_moves(stats_user, pokemon);
		predict_item(stats_user, pokemon);
		predict_ability(stats_user, pokemon);
		optimize_pokemon_evs(pokemon);
	}
	// TODO: This isn't right
	team.pokemon().set_ability_to_base_ability();
	return Team<generation>(team);
}

#define INSTANTIATE(generation) \
	template auto predict_team_impl(StatsUser stats_user, SeenTeam<generation> team) -> Team<generation>

TM_FOR_EACH_GENERATION(INSTANTIATE);

} // namespace technicalmachine
