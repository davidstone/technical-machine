// Generate a random team, weighted by usage
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

#include "random_team.hpp"

#include "estimate.hpp"
#include "load_stats.hpp"
#include "multiplier.hpp"

#include "../team.hpp"

#include "../pokemon/species.hpp"
#include "../string_conversions/conversion.hpp"

#include <bounded_integer/array.hpp>

#include <algorithm>
#include <numeric>
#include <random>

namespace technicalmachine {
namespace {

std::vector<Species> random_species(std::mt19937 & random_engine, Team const & team, unsigned random_pokemon);
}	// namespace


void random_team(Team & team, std::mt19937 & random_engine, unsigned const random_pokemon) {
	for (Species const species : random_species(random_engine, team, random_pokemon)) {
		Level const level(100_bi);
		team.add_pokemon(species, level, Gender::GENDERLESS);
	}
}

namespace {

std::vector<Species> random_species(std::mt19937 & random_engine, Team const & team, unsigned const random_pokemon) {
	auto const overall = overall_stats();
	bounded::array<float, number_of_species> lead;
	lead.fill(1.0F);
	unsigned const total(std::accumulate(std::begin(overall), std::end(overall), 0U));
	Estimate estimate(overall, lead, total);
	Multiplier multiplier(overall);
	estimate.update(multiplier, team);
	std::vector<Species> current;
	for (unsigned generated = 0; generated != random_pokemon; ++generated) {
		Species const species = estimate.random(random_engine);
		estimate.update(multiplier, species);
		current.emplace_back(species);
	}
	return current;
}

}	// namespace

}	// namespace technicalmachine
