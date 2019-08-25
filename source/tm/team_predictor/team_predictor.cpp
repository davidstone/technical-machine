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
#include <tm/pokemon/species.hpp>

#include <containers/algorithms/all_any_none.hpp>
#include <containers/array/make_array.hpp>

namespace technicalmachine {
enum class Moves : std::uint16_t;
namespace {

void predict_pokemon(Team & team, Estimate estimate, Multiplier const & multiplier) {
	auto const index = team.all_pokemon().index();
	while (team.number_of_seen_pokemon() < team.size()) {
		Species const name = estimate.most_likely();
		Level const level(100_bi);
		team.add_pokemon(name, level, Gender::genderless);
		if (team.number_of_seen_pokemon() == team.size())
			break;
		estimate.update(multiplier, name);
	}
	team.all_pokemon().set_index(index);
}

void predict_move(MoveContainer & moves, Generation const generation, DetailedStats::UsedMoves const & detailed) {
	for (Moves const move : detailed) {
		auto const regular = moves.regular();
		if (size(regular) == max_moves_per_pokemon) {
			break;
		}
		if (containers::any_equal(regular, move)) {
			continue;
		}
		containers::emplace_back(moves, generation, move);
	}
}

}	// namespace

Team predict_team(Generation const generation, UsageStats const & usage_stats, LeadStats const lead_stats, Team team, std::mt19937 & random_engine) {
	auto const & multiplier = usage_stats.multiplier();
	auto const & detailed = usage_stats.detailed();
	auto estimate = Estimate(usage_stats, lead_stats);
	estimate.update(multiplier, team);

	predict_pokemon(team, estimate, multiplier);
	for (auto & pokemon : team.all_pokemon()) {
		auto const species = get_species(pokemon);
		if (!ability_is_known(pokemon)) {
			set_ability(pokemon, detailed.get<Ability>(species));
		}
		if (!item_is_known(pokemon)) {
			set_item(pokemon, detailed.get<Item>(species));
		}
		if (!nature_is_known(pokemon)) {
			set_nature(pokemon, detailed.get<Nature>(species));
		}
		predict_move(all_moves(pokemon), generation, detailed.get<DetailedStats::UsedMoves>(species));
		optimize_evs(pokemon, random_engine);
	}
	return team;
}

}	// namespace technicalmachine
