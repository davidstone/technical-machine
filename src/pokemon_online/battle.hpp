// Pokemon Online battle logic
// Copyright (C) 2012 David Stone
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

#ifndef POKEMON_ONLINE__BATTLE_HPP_
#define POKEMON_ONLINE__BATTLE_HPP_

#include <random>
#include <string>
#include <vector>
#include "outmessage.hpp"
#include "../battle.hpp"

namespace technicalmachine {
namespace po {

class Client;
class InMessage;

class Battle : public GenericBattle {
	public:
		Battle (std::random_device::result_type seed, std::string const & opponent_name, unsigned battle_depth, std::string const & team_file_name, bool challenger);
		Battle (std::random_device::result_type seed, std::string const & opponent_name, unsigned battle_depth, Team const & team, bool challenger);
		void handle_message (Client & client, uint32_t battle_id, uint8_t command, uint8_t player, InMessage & msg);
	private:
		void parse_begin_turn (InMessage & msg);
		void parse_send_out (InMessage & msg, uint8_t player);
		void parse_use_attack (InMessage & msg, uint8_t player);
		void parse_straight_damage (InMessage & msg);
		void parse_hp_change (InMessage & msg, uint8_t player);
		int16_t calculate_change_in_hp (bool my_team, uint16_t remaining_hp) const;
		void parse_pp_change (InMessage & msg);
		void parse_effectiveness (InMessage & msg);
		void handle_miss (uint8_t player);
		void handle_critical_hit (uint8_t player);
		void parse_number_of_hits (InMessage & msg);
		void parse_stat_change (InMessage & msg);
		void parse_status_message (InMessage & msg);
		void parse_status_change (InMessage & msg);
		void parse_abs_status_change (InMessage & msg);
		void parse_already_statused (InMessage & msg);
		void handle_flinch (uint8_t player);
		void parse_recoil (InMessage & msg);
		void parse_weather_message (InMessage & msg);
		void parse_ability_message (InMessage & msg, uint8_t player);
		void parse_item_message (InMessage & msg, uint8_t player);
		void parse_move_message (InMessage & msg);
		void parse_substitute (InMessage & msg);
		void parse_dynamic_info (InMessage & msg);
		void parse_dynamic_stats (InMessage & msg);
		void parse_spectating (InMessage & msg);
		void parse_temporary_pokemon_change (InMessage & msg);
		void parse_clock_start (InMessage & msg);
		void parse_clock_stop (InMessage & msg);
		void handle_rated (Client & client, InMessage & msg);
		void parse_tier_section (InMessage & msg);
		void parse_battle_chat (Client const & client, InMessage & msg);
		void parse_spectator_chat (Client const & client, InMessage & msg, uint32_t battle_id);
		void parse_point_estimate (InMessage & msg);
		void parse_offer_choice (Client & client, InMessage & msg, uint32_t battle_id);
		void handle_make_your_choice (Client & client);
		void handle_cancel_move ();
		void parse_rearrange_team (InMessage & msg);
		void parse_spot_shifts (InMessage & msg);
		void parse_battle_end (InMessage & msg);
		uint16_t max_damage_precision () const;
		uint8_t get_target () const;
		int16_t ai_change_in_hp (uint16_t remaining_hp) const;
		int16_t foe_change_in_hp (uint16_t remaining_hp) const;
		static constexpr unsigned pokemon_per_team = 6;
		static constexpr unsigned moves_per_pokemon = 4;
		OutMessage action;
		uint16_t damage;
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE__BATTLE_HPP_
