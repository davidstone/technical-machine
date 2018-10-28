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

#include "../move/move.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include <containers/algorithms/find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <string>
#include <utility>

namespace technicalmachine {

void Battle::handle_use_move(Party const party, uint8_t /*slot*/, Moves const move_name) {
	auto & user = is_me(party) ? m_ai : m_foe;
	auto & other = is_me(party) ? m_foe : m_ai;

	auto const move = add_seen_move(all_moves(user.team.pokemon()), move_name);

	Species const species = user.team.pokemon();
	std::cout << user.team.who() << "'s move: " << to_string(species) << " uses " << to_string(move_name) << '\n';

	constexpr auto other_move = bounded::none;
	constexpr auto miss = false;
	constexpr auto critical_hit = false;
	constexpr auto awakens = false;
	constexpr auto user_damaged = bounded::optional<damage_type>();
	constexpr auto other_damaged = bounded::optional<damage_type>();
	call_move(
		user.team,
		move,
		static_cast<bool>(user_damaged),
		other.team,
		other_move,
		static_cast<bool>(other_damaged),
		m_weather,
		user.variable,
		miss,
		awakens,
		critical_hit,
		other_damaged
	);
}

namespace {

auto index_of_seen(PokemonCollection const & collection, Species const species) {
	using index_type = containers::index_type<PokemonCollection>;
	return static_cast<index_type>(containers::find(collection, species) - begin(collection));
}

auto find_or_add_pokemon(Team & switcher, Species const species, Level const level, Gender const gender) {
	auto const index = index_of_seen(switcher.all_pokemon(), species);
	if (index == switcher.number_of_seen_pokemon()) {
		switcher.all_pokemon().add(species, level, gender);
	}
	return index;
}

}	// namespace

void Battle::handle_send_out(Party const switcher_party, uint8_t const slot, Species const species, Level const level, Gender const gender) {
	auto & switcher = get_team(switcher_party).team;
	auto const index = find_or_add_pokemon(switcher, species, level, gender);

	// TODO: switch_decision_required includes replacing a fainted Pokemon. When
	// replacing a fainted Pokemon, should we call switch_pokemon or call_move?
	// When using Baton Pass and U-turn, should we call switch_pokemon or
	// call_move?
	if (switch_decision_required(switcher.pokemon())) {
		auto & other = get_team(technicalmachine::other(switcher_party)).team;
		switch_pokemon(switcher, other, m_weather, index);
	} else {
		handle_use_move(switcher_party, slot, to_switch(index));
	}
}

void Battle::handle_phaze(Party const phazer_party, uint8_t const phazer_slot, uint8_t /*switcher_slot*/, Moves const move, Species const species, Level const level, Gender const gender) {
	auto & switcher = get_team(technicalmachine::other(phazer_party)).team;
	auto index = find_or_add_pokemon(switcher, species, level, gender);
	static_cast<void>(index);
	handle_use_move(phazer_party, phazer_slot, move);
}

} // namespace technicalmachine
