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

#include <boost/beast/websocket.hpp>

#include <cstdint>
#include <filesystem>
#include <iostream>
#include <random>
#include <string>

namespace technicalmachine {
struct DetailedStats;
struct Multiplier;

struct Battle {
	Battle(OverallStats const & overall, DetailedStats const & detailed, LeadStats const & lead, Multiplier const & multiplier, Evaluate const & evaluate, Party party, std::string opponent, unsigned battle_depth, std::mt19937 random_engine_, Team team, TeamSize foe_size, VisibleFoeHP max_damage_precision = 48_bi);

	Moves determine_action();
	
	bool is_me(Party const other_party) const {
		return my_party == other_party;
	}

	Team predict_foe_team();
	
	auto move_index(Moves const move) const {
		auto const moves = all_moves(ai.team.pokemon()).regular();
		auto const it = containers::find(begin(moves), end(moves), move);
		return it - begin(moves);
	}
	
	std::mt19937 & random_engine() {
		return m_random_engine;
	}
	
	template<typename Integer>
	void handle_begin_turn(Integer const turn_count) {
		std::cout << "Begin turn " << turn_count << '\n';
		if (turn_count != 1_bi) {
			update_from_previous_turn();
		}
		ai.team.reset_between_turns();
		foe.team.reset_between_turns();
		updated_hp.reset_between_turns();
		// Simulators might not send an HP change message if a move does 0 damage.
		move_damage = false;
		
		first = nullptr;
		last = nullptr;
		ai.flags = {};
		foe.flags = {};
	}

	void handle_use_move(Party user, uint8_t slot, Moves move_name);
	void handle_send_out(Party switcher, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, Level level);
	void handle_fainted(Party const fainter, uint8_t /*slot*/) {
		// "slot" is only useful in situations other than 1v1, which TM does not yet
		// support.
		auto const & team = get_team(fainter).team;
		std::cerr << to_string(static_cast<Species>(team.pokemon())) << " fainted\n";
		updated_hp.faint(team.is_me(), team.pokemon());
	}

	void handle_end(Result const result);

	std::string const & opponent() const {
		return opponent_name;
	}

	void handle_hp_change(Party const changing, uint8_t /*slot*/, UpdatedHP::VisibleHP remaining_hp) {
		// "slot" is only useful in NvN, which TM does not yet
		// support.
		auto const & team = get_team(changing).team;
		updated_hp.update(team.is_me(), team.replacement(), remaining_hp);
	}

	bool is_valid_hp_change(Party changer, UpdatedHP::VisibleHP remaining_hp, int received_change) const {
		return hp_change(changer, remaining_hp) == received_change;
	}

	bool is_valid_precision(Party changer, unsigned precision) const {
		return max_visible_hp_change(changer) == precision;
	}

	void handle_direct_damage(Party const damaged, uint8_t slot, UpdatedHP::VisibleHP damage);

	uint8_t switch_slot(Moves move) const;

	int hp_change(Party changing, UpdatedHP::VisibleHP remaining_hp) const;

	using MaxVisibleHPChange = std::common_type<VisibleFoeHP, HP::max_type>::type;
	auto max_visible_hp_change(Party const changer) const -> MaxVisibleHPChange {
		return max_visible_hp_change(get_team(changer).team);
	}

	void handle_flinch(Party const party) {
		set_flinch(get_team(party).variable, true);
	}
	void handle_miss(Party const party) {
		get_team(party).flags.miss = true;
	}

	void handle_critical_hit(Party const party) {
		get_team(party).flags.critical_hit = true;
	}

	void handle_ability_message(Party party, Ability ability) {
		get_ability(get_team(party).team.replacement()) = ability;
	}

	void handle_item_message(Party party, Item item) {
		get_item(get_team(party).team.replacement()) = item;
	}

	void slot_memory_bring_to_front();
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

	auto max_visible_hp_change(Team const & changer) const -> MaxVisibleHPChange {
		return max_visible_hp_change(changer.is_me(), changer.replacement());
	}
	auto max_visible_hp_change(bool const my_pokemon, Pokemon const & changer) const -> MaxVisibleHPChange {
		return my_pokemon ? get_hp(changer).max() : max_damage_precision;
	}
	void do_turn();
	void update_from_previous_turn();

	auto get_team(Party const party) const -> BattleTeam const & {
		return is_me(party) ? ai : foe;
	}
	auto get_team(Party const party) -> BattleTeam & {
		return is_me(party) ? ai : foe;
	}

	OverallStats const & m_overall;
	DetailedStats const & m_detailed;
	LeadStats const & m_lead;
	Multiplier const & m_multiplier;
	Evaluate m_evaluate;
	std::string opponent_name;
	std::mt19937 m_random_engine;
	BattleTeam ai;
	BattleTeam foe;
	containers::static_vector<Species, static_cast<intmax_t>(max_pokemon_per_team)> slot_memory;
	UpdatedHP updated_hp;
	Weather weather;
	BattleTeam * first = nullptr;
	BattleTeam * last = nullptr;
	unsigned depth;
	VisibleFoeHP max_damage_precision;
	bool move_damage = false;
	Party my_party;
};

}	// namespace technicalmachine
