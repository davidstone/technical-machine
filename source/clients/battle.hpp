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
#include "party.hpp"

#include "../endofturn.hpp"
#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../evaluate/evaluate.hpp"

#include "../move/max_moves_per_pokemon.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/species_forward.hpp"

#include "../string_conversions/move.hpp"
#include "../string_conversions/pokemon.hpp"

#include "../team_predictor/load_stats.hpp"

#include <containers/static_vector/static_vector.hpp>

#include <iostream>
#include <random>
#include <string>
#include <utility>

namespace technicalmachine {
struct DetailedStats;
struct Multiplier;

using VisibleFoeHP = bounded::checked_integer<48, 100>;
using VisibleHP = std::common_type_t<VisibleFoeHP, HP::current_type>;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(OverallStats const & overall, DetailedStats const & detailed, LeadStats const & lead, Multiplier const & multiplier, Evaluate const & evaluate, Party party, std::string opponent, unsigned battle_depth, Team team, TeamSize foe_size, VisibleFoeHP max_damage_precision = 48_bi);

	Moves determine_action(std::mt19937 & random_engine) const;
	
	bool is_me(Party const other_party) const {
		return m_ai_party == other_party;
	}

	auto move_index(Moves const move) const {
		auto const container = all_moves(m_ai.team.pokemon()).regular();
		auto const it = containers::find(container, move);
		if (it == end(container)) {
			throw std::runtime_error("AI's active Pokemon does not know " + std::string(to_string(move)));
		}
		return it - begin(container);
	}
	auto species_index(Species const species) const {
		auto const & container = m_ai.team.all_pokemon();
		auto const it = containers::find(container, species);
		if (it == end(container)) {
			throw std::runtime_error("AI's team does not have a " + std::string(to_string(species)));
		}
		return TeamIndex(it - begin(container));
	}
	
	template<typename Integer>
	void handle_begin_turn(Integer const turn_count) {
		std::cout << "Begin turn " << turn_count << '\n';
		if (turn_count != 1_bi) {
			// TODO: This should not be assert
			assert(m_first);
			assert(m_last);
			end_of_turn(m_first->team, m_last->team, m_weather, m_first->flags.shed_skin, m_last->flags.shed_skin);
		}
		
		m_first = nullptr;
		m_last = nullptr;
		m_ai.flags = {};
		m_foe.flags = {};

		std::cout << to_string(m_ai.team) << '\n';
		std::cout << to_string(m_foe.team) << '\n';
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

	void handle_end(Result result, std::mt19937 & random_engine) const;

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
		struct Flags {
			bounded::optional<damage_type> damaged;
			bounded::optional<UsedMove> used_move;
			bool awakens = false;
			bool shed_skin = false;
		};
		
		template<typename... Args>
		BattleTeam(Args && ... args):
			team(std::forward<Args>(args)...)
		{
		}

		Team team;
		Variable variable;
		Flags flags;
	};

	Team predict_foe_team(std::mt19937 & random_engine) const;
	
	auto to_real_hp(bool const my_pokemon, Pokemon const & changer, VisibleHP const visible_remaining_hp) const -> HP::current_type {
		auto const max_hp = get_hp(changer).max();
		auto const max_visible_hp_change = BOUNDED_CONDITIONAL(my_pokemon, max_hp, m_max_damage_precision);
		auto const result = bounded::max(1_bi, max_hp * visible_remaining_hp / max_visible_hp_change);
		if (result.max() > HP::current_type::max()) {
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

	OverallStats const & m_overall;
	DetailedStats const & m_detailed;
	LeadStats const & m_lead;
	Multiplier const & m_multiplier;
	Evaluate m_evaluate;
	std::string m_opponent;
	BattleTeam m_ai;
	BattleTeam m_foe;
	Weather m_weather;
	BattleTeam * m_first = nullptr;
	BattleTeam * m_last = nullptr;
	unsigned m_depth;
	VisibleFoeHP m_max_damage_precision;
	Party m_ai_party;
};

}	// namespace technicalmachine
