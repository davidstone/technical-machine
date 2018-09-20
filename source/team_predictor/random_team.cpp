// Generate a random team, weighted by usage
// Copyright (C) 2015 David Stone
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

#include <containers/array/make_array.hpp>
#include <containers/algorithms/accumulate.hpp>
#include <containers/integer_range.hpp>

#include <algorithm>
#include <numeric>
#include <random>

namespace technicalmachine {
namespace {

}	// namespace

void random_team(Team & team, std::mt19937 & random_engine) {
	auto const overall = overall_stats();
	constexpr auto lead = containers::make_array_n(bounded::constant<number_of_species>, 1.0F);
	auto estimate = Estimate(overall, lead, containers::accumulate(overall));
	auto const multiplier = Multiplier(overall);
	estimate.update(multiplier, team);
	for (auto const n [[maybe_unused]] : containers::integer_range(max_pokemon_per_team - size(team.all_pokemon()))) {
		auto const species = estimate.random(random_engine);
		estimate.update(multiplier, species);
		team.add_pokemon(species, Level(100_bi), Gender::genderless);
	}
}

}	// namespace technicalmachine
