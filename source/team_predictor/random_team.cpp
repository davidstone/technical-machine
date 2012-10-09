// Generate a random team, weighted by usage
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

#include "random_team.hpp"

#include <algorithm>
#include <numeric>
#include <random>

#include "estimate.hpp"
#include "load_stats.hpp"
#include "multiplier.hpp"

#include "../team.hpp"

#include "../pokemon/species.hpp"
#include "../string_conversions/conversion.hpp"

namespace technicalmachine {
namespace {

std::vector<Species> random_species(std::mt19937 & random_engine, Team const & team, unsigned max_random_pokemon);
unsigned pokemon_to_generate(std::mt19937 & random_engine, unsigned max_random_pokemon);
}	// unnamed namespace


void random_team(Team & team, std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	for (Species const species : random_species(random_engine, team, max_random_pokemon)) {
		constexpr uint8_t level = 100;
		team.add_pokemon(species, level, Gender());
	}
}

Team random_team(std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	Team team;
	random_team(team, random_engine, max_random_pokemon);
	return team;
}

namespace {

std::vector<Species> random_species(std::mt19937 & random_engine, Team const & team, unsigned const max_random_pokemon) {
	std::array<unsigned, number_of_species> const overall(overall_stats());
	std::array<float, number_of_species> lead;
	lead.fill(1);
	unsigned const total(std::accumulate(std::begin(overall), std::end(overall), 0U));
	Estimate estimate(overall, lead, total);
	Multiplier multiplier(overall);
	estimate.update(multiplier, team);
	std::vector<Species> current;
	auto const random_pokemon = pokemon_to_generate(random_engine, max_random_pokemon);
	for (unsigned generated = 0; generated != random_pokemon; ++generated) {
		Species const species = estimate.random(random_engine);
		estimate.update(multiplier, species);
		current.emplace_back(species);
	}
	return current;
}

unsigned pokemon_to_generate(std::mt19937 & random_engine, unsigned const max_random_pokemon) {
	std::uniform_int_distribution<unsigned> team_distribution(0, max_random_pokemon);
	return team_distribution(random_engine);
}

}	// unnamed namespace

}	// namespace technicalmachine
