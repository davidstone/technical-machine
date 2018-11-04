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

#include <tm/clients/battle.hpp>

#include <tm/switch.hpp>
#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <tm/move/move.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/use_move.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <string>
#include <utility>

namespace technicalmachine {

void Battle::handle_use_move(Party const party, uint8_t /*slot*/, Moves const move, Variable const & variable, bool const miss, bool const critical_hit, bool const awakens, bounded::optional<damage_type> const damage) {
	auto & user = is_me(party) ? m_ai : m_foe;
	auto & other = is_me(party) ? m_foe : m_ai;

	add_seen_move(all_moves(user.pokemon()), move);

	auto const species = get_species(user.pokemon());
	std::cout << user.who() << "'s move: " << to_string(species) << " uses " << to_string(move) << '\n';

	constexpr auto other_move = bounded::none;
	constexpr auto user_damaged = bounded::optional<damage_type>();
	constexpr auto other_damaged = bounded::optional<damage_type>();
	call_move(
		user,
		move,
		static_cast<bool>(user_damaged),
		other,
		other_move,
		static_cast<bool>(other_damaged),
		m_weather,
		variable,
		miss,
		awakens,
		critical_hit,
		damage
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
	auto & switcher = get_team(switcher_party);
	auto const index = find_or_add_pokemon(switcher, species, level, gender);

	// TODO: switch_decision_required includes replacing a fainted Pokemon. When
	// replacing a fainted Pokemon, should we call switch_pokemon or call_move?
	// When using Baton Pass and U-turn, should we call switch_pokemon or
	// call_move?
	if (switch_decision_required(switcher.pokemon())) {
		auto & other = get_team(technicalmachine::other(switcher_party));
		switch_pokemon(switcher, other, m_weather, index);
	} else {
		constexpr auto miss = false;
		constexpr auto critical_hit = false;
		constexpr auto awakens = false;
		handle_use_move(switcher_party, slot, to_switch(index), Variable{}, miss, critical_hit, awakens, bounded::none);
	}
}

void Battle::add_pokemon_from_phaze(Party const party, uint8_t /*slot*/, Species const species, Level const level, Gender const gender) {
	find_or_add_pokemon(get_team(party), species, level, gender);
}

} // namespace technicalmachine
