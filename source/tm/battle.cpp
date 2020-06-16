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

#include <tm/battle.hpp>

#include <tm/move/call_move.hpp>
#include <tm/move/move.hpp>

#include <tm/string_conversions/status.hpp>

#include <tm/team.hpp>
#include <tm/variable.hpp>
#include <tm/weather.hpp>

#include <containers/algorithms/find.hpp>
#include <containers/index_type.hpp>

#include <iostream>
#include <string>
#include <utility>

namespace technicalmachine {

enum class Moves : std::uint16_t;

void Battle::handle_use_move(bool const is_ai, UsedMove const move, bool const clear_status, ActualDamage const damage, OtherMove const other_move) {
	auto & user = is_ai ? m_ai : m_foe;
	auto & other = is_ai ? m_foe : m_ai;

	add_seen_move(all_moves(user.pokemon()), m_generation, move.selected);
	// TODO: Add move.executed in some circumstances

	call_move(
		m_generation,
		user,
		move,
		other,
		other_move,
		m_weather,
		clear_status,
		damage
	);
}

auto Battle::find_or_add_pokemon(bool const is_ai, Species const species, Level const level, Gender const gender) -> Moves {
	auto & switcher = is_ai ? m_ai : m_foe;
	auto const index = find_index(switcher.all_pokemon(), species);
	if (index == switcher.number_of_seen_pokemon()) {
		switcher.all_pokemon().add(m_generation, species, level, gender);
	}
	return to_switch(index);
}

auto Battle::correct_hp(bool const is_ai, HP const original_hp, VisibleHP const visible_hp) -> HP::current_type {
	auto const current_hp = original_hp.current();
	auto const seen_hp = to_real_hp(is_ai, original_hp, visible_hp);
	if (seen_hp.min > current_hp or seen_hp.max < current_hp) {
		// TODO: Find a better way to sync this up with server messages. Find a
		// better way to fail unit tests if this happens.
		std::cerr << "HP out of sync with server messages. Expected " << current_hp << " but visible HP is between " << seen_hp.min << " and " << seen_hp.max << " (max of " << original_hp.max() << ")\n";
	}
	return seen_hp.value;
}

void Battle::validate_status(Statuses const original_status, Statuses const visible_status) {
	auto const normalized_original_status = (original_status == Statuses::rest) ? Statuses::sleep : original_status;
	if (normalized_original_status != visible_status) {
		throw std::runtime_error(
			"Status out of sync with server messages: expected " +
			std::string(to_string(original_status)) +
			" but received " +
			std::string(to_string(visible_status))
		);
	}
}

} // namespace technicalmachine
