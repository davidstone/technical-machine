// Generic battle
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

#include "battle.hpp"

#include "../switch.hpp"
#include "../team.hpp"
#include "../weather.hpp"

#include "../evaluate/expectiminimax.hpp"

#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include "../team_predictor/team_predictor.hpp"

#include <containers/algorithms/find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
struct DetailedStats;

Team Battle::predict_foe_team(std::mt19937 & random_engine) const {
	return predict_team(m_usage_stats, use_lead_stats, m_foe.team, random_engine);
}

Moves Battle::determine_action(std::mt19937 & random_engine) const {
	if (m_ai.team.size() == 0_bi or m_foe.team.size() == 0_bi) {
		std::cerr << "Tried to determine an action with an empty team.\n";
		return Moves::Struggle;
	}

	std::cout << std::string(20, '=') + '\n';
	std::cout << "Predicting...\n";
	auto predicted = predict_foe_team(random_engine);
	//std::cout << to_string(predicted) << '\n';

	return expectiminimax(m_ai.team, predicted, m_weather, m_depth, m_evaluate);
}

void Battle::handle_use_move(Party const party, uint8_t /*slot*/, Moves const move_name) {
	auto & user = is_me(party) ? m_ai : m_foe;
	auto & other = is_me(party) ? m_foe : m_ai;

	if (m_first == nullptr) {
		m_first = std::addressof(user);
		m_last = std::addressof(other);
	}

	auto const move = add_seen_move(all_moves(user.team.pokemon()), move_name);

	Species const species = user.team.pokemon();
	std::cout << user.team.who() << "'s move: " << to_string(species) << " uses " << to_string(move_name) << '\n';

	constexpr auto other_move = bounded::none;
	constexpr auto miss = false;
	constexpr auto critical_hit = false;
	call_move(user.team, move, static_cast<bool>(user.flags.damaged), other.team, other_move, static_cast<bool>(other.flags.damaged), m_weather, user.variable, miss, user.flags.awakens, critical_hit, other.flags.damaged);
}

namespace {

auto index_of_seen(PokemonCollection const & collection, Species const species) {
	using index_type = containers::index_type<PokemonCollection>;
	return static_cast<index_type>(containers::find(collection, species) - begin(collection));
}

template<typename... Args>
void switch_or_add(Team & switcher, Team & other, Weather & weather, Species const species, Args && ... args) {
	auto const index = index_of_seen(switcher.all_pokemon(), species);
	if (index == switcher.number_of_seen_pokemon()) {
		switcher.all_pokemon().add(species, std::forward<Args>(args)...);
	}
	// TODO: Call this only when a switch decision is required, not for the case
	// where a Pokemon used "Switch" as its move.
	switch_pokemon(switcher, other, weather, index);
}

}	// namespace

void Battle::handle_send_out(Party const switcher_party, uint8_t /*slot*/, std::string const & nickname, Species const species, Gender const gender, Level const level) {
	auto & switcher = get_team(switcher_party);
	auto & other = get_team(technicalmachine::other(switcher_party));

	if (m_first == nullptr) {
		m_first = std::addressof(switcher);
		m_last = std::addressof(other);
	}

	// This assumes Species Clause is in effect
	switch_or_add(switcher.team, other.team, m_weather, species, level, gender, nickname);
	
	if (other.team.number_of_seen_pokemon() != 0_bi and other.flags.used_move and is_phaze(other.flags.used_move->move)) {
		set_phaze_index(other.variable, switcher.team, species);
	} else if (!moved(switcher.team.pokemon())) {
		switcher.flags.used_move.emplace(Move(to_switch(switcher.team.all_pokemon().index())), 0_bi);
	}
}

} // namespace technicalmachine
