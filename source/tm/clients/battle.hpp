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

#include <tm/move/damage_type.hpp>
#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/other_move.hpp>
#include <tm/move/used_move.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <tm/endofturn.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
struct UsageStats;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(
		Party const party,
		Team ai_,
		Team foe_
	):
		m_ai(std::move(ai_)),
		m_foe(std::move(foe_)),
		m_ai_party(party)
	{
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

	// TODO: require users to specify which team went first and whether Shed
	// Skin activated
	template<typename Integer>
	void handle_begin_turn(Integer const turn_count) {
		if (turn_count != 1_bi) {
			constexpr auto ai_shed_skin_activated = false;
			constexpr auto foe_shed_skin_activated = false;
			constexpr auto ai_lock_in_ended = false;
			constexpr auto foe_lock_in_ended = false;
			end_of_turn(m_ai, ai_shed_skin_activated, ai_lock_in_ended, m_foe, foe_shed_skin_activated, foe_lock_in_ended, m_weather);
		} else {
			for (auto side : {std::ref(m_ai), std::ref(m_foe)}) {
				side.get().pokemon().set_not_moved();
			}
		}
	}

	void handle_use_move(Party user, uint8_t slot, UsedMove move, bool clear_status, bounded::optional<damage_type> damage, OtherMove other_move);
	// This assumes Species Clause is in effect. This does not perform any
	// switching, it just adds them to the team.
	auto find_or_add_pokemon(Party const party, uint8_t slot, Species species, Level level, Gender gender) -> Moves;
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		auto & team = get_team(fainter);
		auto active_pokemon = team.pokemon();
		get_hp(active_pokemon) = 0_bi;
		active_pokemon.faint();
	}

	void set_value_on_active(Party const party, Ability const ability) {
		set_ability(get_team(party).pokemon(), ability);
	}

	void set_value_on_active(Party const party, Item const item) {
		set_item(get_team(party).pokemon(), item);
	}

	// maybe_index is either an index into a PokemonCollection or nothing
	template<typename... MaybeIndex>	
	auto & hp(Party const party, MaybeIndex... maybe_index) {
		return get_hp(get_team(party).pokemon(maybe_index...));
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
	Party m_ai_party;
};

}	// namespace technicalmachine
