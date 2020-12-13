// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tm/team_predictor/estimate.hpp>

#include <tm/pokemon/level.hpp>
#include <tm/pokemon/max_pokemon_per_team.hpp>

#include <tm/gender.hpp>
#include <tm/generation.hpp>

#include <containers/integer_range.hpp>
#include <containers/size.hpp>

#include <random>

namespace technicalmachine {

template<Generation>
struct Pokemon;
template<Generation>
struct Team;
struct UsageStats;

template<Generation generation>
void update_estimate(Estimate & estimate, UsageStats const & usage_stats, Team<generation> const & team) {
	for (auto const & pokemon : team.all_pokemon()) {
		estimate.update(usage_stats, pokemon.species());
	}
}

// Fill an existing team with random Pokemon, weighted by usage
template<Generation generation>
void random_team(UsageStats const & usage_stats, Team<generation> & team, std::mt19937 & random_engine) {
	auto estimate = Estimate(usage_stats, do_not_use_lead_stats);
	update_estimate(estimate, usage_stats, team);
	for (auto const n [[maybe_unused]] : containers::integer_range(max_pokemon_per_team - containers::size(team.all_pokemon()))) {
		auto const species = estimate.random(random_engine);
		estimate.update(usage_stats, species);
		team.add_pokemon(Pokemon<generation>(species, Level(100_bi), Gender::genderless));
	}
}

}	// namespace technicalmachine
