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

#include <tm/move/call_move.hpp>
#include <tm/move/move.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <string>
#include <utility>

namespace technicalmachine {

enum class Moves : std::uint16_t;

void Battle::handle_use_move(Party const party, uint8_t /*slot*/, UsedMove const move, bool const clear_status, bounded::optional<damage_type> const damage, OtherMove const other_move) {
	auto & user = is_me(party) ? m_ai : m_foe;
	auto & other = is_me(party) ? m_foe : m_ai;

	add_seen_move(all_moves(user.pokemon()), move.selected);
	// TODO: Add move.executed in some circumstances

	call_move(
		user,
		move,
		other,
		other_move,
		m_weather,
		clear_status,
		damage
	);
}

namespace {

auto index_of_seen(PokemonCollection const & collection, Species const species) {
	using index_type = containers::index_type<PokemonCollection>;
	return static_cast<index_type>(containers::find(collection, species) - begin(collection));
}

}	// namespace

auto Battle::find_or_add_pokemon(Party const party, uint8_t /*slot*/, Species const species, Level const level, Gender const gender) -> Moves {
	auto & switcher = get_team(party);
	auto const index = index_of_seen(switcher.all_pokemon(), species);
	if (index == switcher.number_of_seen_pokemon()) {
		switcher.all_pokemon().add(species, level, gender);
	}
	return to_switch(index);
}

} // namespace technicalmachine
