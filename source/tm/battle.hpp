// Copyright (C) 2019 David Stone
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

#pragma once

#include <tm/clients/battle_result.hpp>
#include <tm/clients/party.hpp>

#include <tm/move/actual_damage.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <tm/end_of_turn.hpp>
#include <tm/team.hpp>
#include <tm/visible_hp.hpp>
#include <tm/weather.hpp>

#include <cstdint>
#include <utility>

namespace technicalmachine {

enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(
		Generation const generation,
		Team ai_,
		Team foe_
	):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_)),
		m_generation(generation)
	{
		// TODO: Properly order this
		m_ai.pokemon().switch_in(m_generation, m_weather);
		m_foe.pokemon().switch_in(m_generation, m_weather);
	}
	
	auto generation() const {
		return m_generation;
	}
	auto const & ai() const {
		return m_ai;
	}
	auto const & foe() const {
		return m_foe;
	}
	auto weather() const {
		return m_weather;
	}

	void handle_begin_turn() & {
		m_ai.reset_start_of_turn();
		m_foe.reset_start_of_turn();
	}
	// TODO: require users to specify which team went first and whether Shed
	// Skin activated
	void handle_end_turn() & {
		constexpr auto ai_flags = EndOfTurnFlags{false, false};
		constexpr auto foe_flags = EndOfTurnFlags{false, false};
		end_of_turn(m_generation, m_ai, ai_flags, m_foe, foe_flags, m_weather);
	}

	void handle_use_move(bool is_ai, uint8_t slot, UsedMove move, bool clear_status, ActualDamage visible_damage, OtherMove other_move);
	// This assumes Species Clause is in effect. This does not perform any
	// switching, it just adds them to the team.
	auto find_or_add_pokemon(bool is_ai, uint8_t slot, Species species, Level level, Gender gender) -> Moves;
	void handle_fainted(bool const is_ai, uint8_t /*slot*/) {
		active_pokemon(is_ai).set_hp(0_bi);
	}

	void set_value_on_active(bool const is_ai, Ability const ability) {
		(is_ai ? m_ai : m_foe).pokemon().set_base_ability(ability);
	}

	void set_value_on_active(bool const is_ai, Item const item) {
		Pokemon & pokemon = active_pokemon(is_ai);
		pokemon.set_item(item);
	}

	// TODO: What happens here if a Pokemon has a pinch item?
	void correct_hp_and_status(bool const is_ai, VisibleHP const visible_hp, Statuses const visible_status, auto... maybe_index) {
		auto & pokemon = active_pokemon(is_ai, maybe_index...);
		auto const original_hp = get_hp(pokemon);
		pokemon.set_hp(correct_hp(is_ai, original_hp, visible_hp));
		if (visible_hp.current == 0_bi) {
			return;
		}
		validate_status(get_status(pokemon).name(), visible_status);
	}

private:
	// maybe_index is either an index into a PokemonCollection or nothing
	auto active_pokemon(bool const is_ai, auto... maybe_index) -> Pokemon & {
		return (is_ai ? m_ai : m_foe).pokemon(maybe_index...);
	}
	auto correct_hp(bool is_ai, HP, VisibleHP) -> HP::current_type;
	void validate_status(Statuses const original_status, Statuses const visible_status);

	Team m_ai;
	Team m_foe;
	Weather m_weather;
	Generation m_generation;
};

}	// namespace technicalmachine
