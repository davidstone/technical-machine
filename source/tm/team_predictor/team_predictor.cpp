// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

module;

#include <tm/for_each_generation.hpp>

export module tm.team_predictor.team_predictor;

import tm.pokemon.max_pokemon_per_team;

import tm.team_predictor.stats_user;
import tm.team_predictor.usage_stats;

import tm.generation;
import tm.team;

import std_module;

namespace technicalmachine {

template<Generation generation>
auto predict_team_impl(StatsUser stats_user, std::mt19937 & random_engine, SeenTeam<generation> team) -> Team<generation>;

#define EXTERN_INSTANTIATION(generation) \
	extern template auto predict_team_impl(StatsUser stats_user, std::mt19937 & random_engine, SeenTeam<generation> team) -> Team<generation>

TM_FOR_EACH_GENERATION(EXTERN_INSTANTIATION);

export template<Generation generation>
auto most_likely_team(UsageStats const & usage_stats, std::mt19937 & random_engine, SeenTeam<generation> team) -> Team<generation> {
	return predict_team_impl(StatsUser(usage_stats), random_engine, team);
}

export template<Generation generation>
auto random_team(UsageStats const & usage_stats, std::mt19937 & random_engine, SeenTeam<generation> team = SeenTeam<generation>(max_pokemon_per_team)) -> Team<generation> {
	return predict_team_impl(StatsUser(usage_stats, random_engine), random_engine, team);
}

} // namespace technicalmachine
