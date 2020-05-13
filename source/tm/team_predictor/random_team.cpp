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

#include <tm/team_predictor/random_team.hpp>

#include <tm/team_predictor/estimate.hpp>

#include <tm/team.hpp>

#include <containers/array/make_array.hpp>
#include <containers/algorithms/accumulate.hpp>
#include <containers/integer_range.hpp>

#include <random>

namespace technicalmachine {

void random_team(Generation const generation, UsageStats const & usage_stats, Team & team, std::mt19937 & random_engine) {
	auto estimate = Estimate(usage_stats, do_not_use_lead_stats);
	update_estimate(estimate, usage_stats, team);
	for (auto const n [[maybe_unused]] : containers::integer_range(max_pokemon_per_team - size(team.all_pokemon()))) {
		auto const species = estimate.random(random_engine);
		estimate.update(usage_stats, species);
		team.add_pokemon(generation, species, Level(100_bi), Gender::genderless);
	}
}

}	// namespace technicalmachine
