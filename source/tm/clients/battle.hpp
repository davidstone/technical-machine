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

#pragma once

#include <tm/clients/battle_result.hpp>
#include <tm/clients/handle_battle_end.hpp>
#include <tm/clients/party.hpp>

#include <tm/endofturn.hpp>
#include <tm/team.hpp>
#include <tm/weather.hpp>

#include <tm/move/max_moves_per_pokemon.hpp>
#include <tm/move/moves.hpp>
#include <tm/move/use_move.hpp>

#include <tm/pokemon/species_forward.hpp>

#include <tm/string_conversions/move.hpp>
#include <tm/string_conversions/pokemon.hpp>

#include <iostream>
#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
struct UsageStats;
struct Variable;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(
		Party const party,
		std::string foe_name_,
		Team ai_,
		Team foe_
	):
		m_foe_name(std::move(foe_name_)),
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
	auto foe_name() const -> std::string_view {
		return m_foe_name;
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
		std::cout << "Begin turn " << turn_count << '\n';
		if (turn_count != 1_bi) {
			constexpr auto ai_shed_skin_activated = false;
			constexpr auto foe_shed_skin_activated = false;
			end_of_turn(m_ai, m_foe, m_weather, ai_shed_skin_activated, foe_shed_skin_activated);
		} else {
			for (auto side : {std::ref(m_ai), std::ref(m_foe)}) {
				side.get().pokemon().set_not_moved();
			}
		}
		
		std::cout << to_string(ai()) << '\n';
		std::cout << to_string(foe()) << '\n';
	}

	void handle_use_move(Party user, uint8_t slot, Moves move, Variable variable, bool miss, bool critical_hit, bool awakens, bounded::optional<damage_type> damage);
	// This handles direct switches, replacing fainted Pokemon, and switching
	// due to U-turn and Baton Pass. This assumes Species Clause is in effect.
	void handle_send_out(Party switcher, uint8_t slot, Species species, Level level, Gender gender);
	// This assumes Species Clause is in effect. This is intended to be used
	// with phazing moves only. It does not perform any switching, it just adds
	// them to the team.
	void add_pokemon_from_phaze(Party const party, uint8_t slot, Species species, Level level, Gender gender);
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		auto & team = get_team(fainter);
		auto active_pokemon = team.pokemon();
		get_hp(active_pokemon) = 0_bi;
		active_pokemon.faint();
	}

	void set_value_on_active(Party const party, Ability const ability) {
		get_ability(get_team(party).pokemon()) = ability;
	}

	void set_value_on_active(Party const party, Item const item) {
		get_item(get_team(party).pokemon()) = item;
	}
private:
	auto get_team(Party const party) const -> Team const & {
		return is_me(party) ? m_ai : m_foe;
	}
	auto get_team(Party const party) -> Team & {
		return is_me(party) ? m_ai : m_foe;
	}
	
	std::string m_foe_name;
	Team m_ai;
	Team m_foe;
	Weather m_weather;
	Party m_ai_party;
};

}	// namespace technicalmachine
