// Generic battle
// Copyright (C) 2015 David Stone
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

#include "../move/moves.hpp"

#include "../pokemon/species_forward.hpp"

#include <boost/filesystem/path.hpp>

#include <cstdint>
#include <map>
#include <random>
#include <string>
#include <vector>

namespace technicalmachine {
struct Client;
struct DetailedStats;
struct Evaluate;
struct Gender;
struct Level;
namespace network {
struct OutMessage;
} // namespace network

struct Battle {
	bool is_me(Party other_party) const;
	void set_party_if_unknown(Party new_party);
	void write_team(network::OutMessage & msg, std::string const & username = std::string()) const;
	Team predict_foe_team(DetailedStats const & detailed) const;
	void handle_begin_turn(uint16_t turn_count) const;
	void handle_request_action(DetailedStats const & detailed, Evaluate const & evaluate, network::OutMessage & msg, uint32_t battle_id, bool can_switch, std::vector <uint8_t> const & attacks_allowed, bool forced = false);
	void handle_use_move(Party user, uint8_t slot, Moves move_name);
	void handle_send_out(Party switcher, uint8_t slot, uint8_t index, std::string const & nickname, Species species, Gender gender, Level level);
	void handle_set_pp(Party changer, uint8_t slot, uint8_t pp);
	void handle_fainted(Party fainter, uint8_t slot);
	void handle_end(Client const & client, Result const result) const;
	std::string const & opponent() const;
	Battle(Battle const &) = delete;
	Battle & operator=(Battle const &) = delete;
	void handle_hp_change(Party changer, uint8_t slot, UpdatedHP::VisibleHP remaining_hp);
	bool is_valid_hp_change(Party changer, UpdatedHP::VisibleHP remaining_hp, int received_change) const;
	bool is_valid_precision(Party changer, unsigned precision) const;
	void handle_direct_damage(Party const damaged, uint8_t slot, UpdatedHP::VisibleHP damage);
	virtual ~Battle() {}
protected:
	Battle(std::string const & opponent, TeamSize foe_size, std::random_device::result_type seed, unsigned battle_depth, boost::filesystem::path const & team_file);
	Battle(std::string const & opponent, TeamSize foe_size, std::random_device::result_type seed, unsigned battle_depth, Team const & team);
	uint8_t switch_slot(Moves move) const;
	virtual VisibleFoeHP max_damage_precision() const;
	void initialize_turn();
	int hp_change(Party changing, UpdatedHP::VisibleHP remaining_hp) const;
	using MaxVisibleHPChange = std::common_type<VisibleFoeHP, HP::max_type>::type;
	MaxVisibleHPChange max_visible_hp_change(Party changer) const;
	void handle_flinch(Party party);
	void handle_miss(Party party);
	void handle_critical_hit(Party party);
	void handle_ability_message(Party party, Ability::Abilities ability);
	void handle_item_message(Party party, Item item);
	void slot_memory_bring_to_front();
private:
	void initialize();
	Moves determine_action(DetailedStats const & detailed, Evaluate const & evaluate);
	void correct_hp_and_report_errors(Team & team);
	void normalize_hp();
	void normalize_hp(Team & team);
	MaxVisibleHPChange max_visible_hp_change(Team const & changer) const;
	MaxVisibleHPChange max_visible_hp_change(bool my_pokemon, Pokemon const & changer) const;
	void do_turn();
	void update_from_previous_turn();
	Team const & get_team(Party party) const;
	Team & get_team(Party party);
	Variable const & variable(Team const & team) const;
	Variable & variable(Team const & team);
	void register_damage();

	std::string opponent_name;
	mutable std::mt19937 random_engine;
	Team ai;
	Team foe;
	std::vector <Species> slot_memory;
	UpdatedHP updated_hp;
	Weather weather;
	Team * first;
	Team * last;
	Variable ai_variable;
	Variable foe_variable;
	unsigned depth;
	bool move_damage;
	Party my_party;
};

}	// namespace technicalmachine
