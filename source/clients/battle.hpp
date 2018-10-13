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
#include "updated_hp.hpp"

#include "../team.hpp"
#include "../variable.hpp"
#include "../weather.hpp"

#include "../evaluate/evaluate.hpp"

#include "../move/max_moves_per_pokemon.hpp"
#include "../move/moves.hpp"
#include "../move/use_move.hpp"

#include "../pokemon/species_forward.hpp"

#include "../string_conversions/pokemon.hpp"

#include "../team_predictor/load_stats.hpp"

#include <containers/static_vector/static_vector.hpp>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
struct DetailedStats;
struct Multiplier;

// In all of these functions, "slot" is useful only in NvN, which TM does not
// yet support.

struct Battle {
	Battle(OverallStats const & overall, DetailedStats const & detailed, LeadStats const & lead, Multiplier const & multiplier, Evaluate const & evaluate, Party party, std::string opponent, unsigned battle_depth, Team team, TeamSize foe_size, VisibleFoeHP max_damage_precision = 48_bi);

	Team predict_foe_team(std::mt19937 & random_engine) const;
	
	Moves determine_action(std::mt19937 & random_engine) const;
	
	bool is_me(Party const other_party) const {
		return m_ai_party == other_party;
	}

	auto move_index(Moves const move) const {
		auto const moves = all_moves(m_ai.team.pokemon()).regular();
		auto const it = containers::find(moves, move);
		return it - begin(moves);
	}
	// Requires that the species is actually on the AI's team
	auto species_index(Species const species) const {
		auto const & container = m_ai.team.all_pokemon();
		auto const it = containers::find(container, species);
		assert(it != end(container));
		return TeamIndex(it - begin(container));
	}
	
	template<typename Integer>
	void handle_begin_turn(Integer const turn_count) {
		std::cout << "Begin turn " << turn_count << '\n';
		if (turn_count != 1_bi) {
			do_turn();
			assert(m_first);
			assert(m_last);
			correct_hp_and_report_errors(m_first->team);
			correct_hp_and_report_errors(m_last->team);
		}
		m_updated_hp.reset_between_turns();
		
		m_first = nullptr;
		m_last = nullptr;
		m_ai.flags = {};
		m_foe.flags = {};
	}

	void handle_use_move(Party user, uint8_t slot, Moves move_name);
	void handle_send_out(Party switcher, uint8_t slot, std::string const & nickname, Species species, Gender gender, Level level);
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		auto const & team = get_team(fainter).team;
		std::cerr << to_string(static_cast<Species>(team.pokemon())) << " fainted\n";
		m_updated_hp.faint(team.is_me(), team.pokemon());
	}

	void handle_end(Result result, std::mt19937 & random_engine) const;

	void handle_hp_change(Party const changing, uint8_t /*slot*/, UpdatedHP::VisibleHP const remaining_hp) {
		auto const & team = get_team(changing).team;
		m_updated_hp.update(team.is_me(), team.replacement(), remaining_hp);
	}

	void handle_direct_damage(Party const damaged, uint8_t slot, UpdatedHP::VisibleHP damage);

	void handle_flinch(Party const party) {
		set_flinch(get_team(party).variable, true);
	}
	void handle_miss(Party const party) {
		get_team(party).flags.miss = true;
	}

	void handle_critical_hit(Party const party) {
		get_team(party).flags.critical_hit = true;
	}

	void handle_ability_message(Party const party, Ability const ability) {
		get_ability(get_team(party).team.replacement()) = ability;
	}

	void handle_item_message(Party const party, Item const item) {
		get_item(get_team(party).team.replacement()) = item;
	}
private:
	struct BattleTeam {
		struct Flags {
			bounded::optional<damage_type> damaged;
			bounded::optional<UsedMove> used_move;
			bool awakens = false;
			bool critical_hit = false;
			bool miss = false;
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


	void correct_hp_and_report_errors(Team & team);
	void normalize_hp();
	void normalize_hp(Team & team);
	
	auto max_visible_hp_change(bool const my_pokemon, Pokemon const & changer) const {
		return BOUNDED_CONDITIONAL(my_pokemon, get_hp(changer).max(), m_max_damage_precision);
	}

	void do_turn();

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
	UpdatedHP m_updated_hp;
	Weather m_weather;
	BattleTeam * m_first = nullptr;
	BattleTeam * m_last = nullptr;
	unsigned m_depth;
	VisibleFoeHP m_max_damage_precision;
	Party m_ai_party;
};

}	// namespace technicalmachine
