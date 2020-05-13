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
#include <tm/weather.hpp>

#include <random>
#include <string>
#include <utility>

namespace technicalmachine {

enum class Generation : std::uint8_t;
enum class Moves : std::uint16_t;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(
		Generation const generation,
		Party const party,
		Team ai_,
		Team foe_
	):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_)),
		m_generation(generation),
		m_ai_party(party)
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

	bool is_me(Party const other_party) const {
		return m_ai_party == other_party;
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

	// maybe_index is either an index into a PokemonCollection or nothing
	auto active_pokemon(Party const party, auto... maybe_index) -> Pokemon & {
		return get_team(party).pokemon(maybe_index...);
	}

	void handle_use_move(Party user, uint8_t slot, UsedMove move, bool clear_status, ActualDamage visible_damage, OtherMove other_move);
	// This assumes Species Clause is in effect. This does not perform any
	// switching, it just adds them to the team.
	auto find_or_add_pokemon(Party const party, uint8_t slot, Species species, Level level, Gender gender) -> Moves;
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		active_pokemon(fainter).set_hp(0_bi);
	}

	void set_value_on_active(Party const party, Ability const ability) {
		get_team(party).pokemon().set_base_ability(ability);
	}

	void set_value_on_active(Party const party, Item const item) {
		Pokemon & pokemon = get_team(party).pokemon();
		pokemon.set_item(item);
	}

private:
	auto get_team(Party const party) const -> Team const & {
		return is_me(party) ? m_ai : m_foe;
	}
	auto get_team(Party const party) -> Team & {
		return is_me(party) ? m_ai : m_foe;
	}
	
	Team m_ai;
	Team m_foe;
	Weather m_weather;
	Generation m_generation;
	Party m_ai_party;
};

}	// namespace technicalmachine
