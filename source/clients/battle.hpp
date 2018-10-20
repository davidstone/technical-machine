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

#include "battle_result.hpp"
#include "handle_battle_end.hpp"
#include "party.hpp"

#include "../endofturn.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../move/max_moves_per_pokemon.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/species_forward.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include <iostream>
#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
struct UsageStats;

using VisibleFoeHP = bounded::checked_integer<48, 100>;
using VisibleHP = std::common_type_t<VisibleFoeHP, HP::current_type>;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(
		Party const party,
		std::string foe_name_,
		Team team,
		TeamSize const foe_size,
		VisibleFoeHP const max_damage_precision_ = 48_bi
	):
		m_foe_name(std::move(foe_name_)),
		m_ai(std::move(team)),
		m_foe(foe_size),
		m_max_damage_precision(max_damage_precision_),
		m_ai_party(party)
	{
	}
	
	auto const & ai() const {
		return m_ai.team;
	}
	auto const & foe() const {
		return m_foe.team;
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
			end_of_turn(m_ai.team, m_foe.team, m_weather, ai_shed_skin_activated, foe_shed_skin_activated);
		}
		
		std::cout << to_string(ai()) << '\n';
		std::cout << to_string(foe()) << '\n';
	}

	// TODO: Require passing in all flags associated with this particular use
	// of the move, such as damage and CH
	void handle_use_move(Party user, uint8_t slot, Moves move_name);
	void handle_send_out(Party switcher, uint8_t slot, std::string const & nickname, Species species, Gender gender, Level level);
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		auto & team = get_team(fainter).team;
		auto active_pokemon = team.pokemon();
		get_hp(active_pokemon) = 0_bi;
		active_pokemon.faint();
	}

	void handle_hp_change(Party const changing, uint8_t /*slot*/, VisibleHP const visible_remaining_hp) {
		auto & team = get_team(changing).team;
		Pokemon & pokemon = team.pokemon();
		get_hp(pokemon) = to_real_hp(team.is_me(), pokemon, visible_remaining_hp);
	}

	void handle_flinch(Party const party) {
		set_flinch(get_team(party).variable, true);
	}

	void handle_ability_message(Party const party, Ability const ability) {
		get_ability(get_team(party).team.pokemon()) = ability;
	}

	void handle_item_message(Party const party, Item const item) {
		get_item(get_team(party).team.pokemon()) = item;
	}
private:
	struct BattleTeam {
		template<typename... Args>
		BattleTeam(Args && ... args):
			team(std::forward<Args>(args)...)
		{
		}

		Team team;
		Variable variable;
	};

	auto to_real_hp(bool const my_pokemon, Pokemon const & changer, VisibleHP const visible_remaining_hp) const -> HP::current_type {
		auto const max_hp = get_hp(changer).max();
		auto const max_visible_hp_change = BOUNDED_CONDITIONAL(my_pokemon, max_hp, m_max_damage_precision);
		auto const result = bounded::max(1_bi, max_hp * visible_remaining_hp / max_visible_hp_change);
		if (result > HP::current_type::max()) {
			throw std::runtime_error("Recieved an HP value that is too large.");
		}
		return HP::current_type(result);
	}

	auto get_team(Party const party) const -> BattleTeam const & {
		return is_me(party) ? m_ai : m_foe;
	}
	auto get_team(Party const party) -> BattleTeam & {
		return is_me(party) ? m_ai : m_foe;
	}

	std::string m_foe_name;
	BattleTeam m_ai;
	BattleTeam m_foe;
	Weather m_weather;
	VisibleFoeHP m_max_damage_precision;
	Party m_ai_party;
};

}	// namespace technicalmachine
