// Pokemon Online battle logic
// Copyright (C) 2013 David Stone
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
#include "outmessage.hpp"
#include "../battle.hpp"

namespace technicalmachine {
namespace po {

class Client;
class InMessage;

class Battle : public ::technicalmachine::Battle {
	public:
		template<typename ... Args>
		Battle(std::random_device::result_type seed, std::string const & foe_name, unsigned const battle_depth, bool const challenger, Args && ... args):
			::technicalmachine::Battle::Battle(seed, foe_name, battle_depth, std::forward<Args>(args)...),
			action (OutMessage::BATTLE_MESSAGE)
			{
			set_party_if_unknown(Party(challenger ? 0 : 1));
		}
		void handle_message (Client & client, uint32_t battle_id, uint8_t command, Party party, InMessage & msg);
		static constexpr unsigned pokemon_per_team() {
			return 6;
		}
		static constexpr unsigned moves_per_pokemon() {
			return 4;
		}
	private:
		void parse_begin_turn (InMessage & msg) const;
		void parse_send_out (InMessage & msg, Party party);
		void parse_use_attack (InMessage & msg, Party party);
		void parse_straight_damage (InMessage & msg);
		void parse_hp_change (InMessage & msg, Party party);
		void parse_pp_change (InMessage & msg);
		void parse_effectiveness (InMessage & msg);
		void parse_number_of_hits (InMessage & msg);
		void parse_stat_change (InMessage & msg);
		void parse_status_message (InMessage & msg);
		void parse_status_change (InMessage & msg);
		void parse_abs_status_change (InMessage & msg);
		void parse_already_statused (InMessage & msg);
		void parse_recoil (InMessage & msg);
		void parse_weather_message (InMessage & msg);
		void parse_ability_message (InMessage & msg, Party party);
		void parse_item_message (InMessage & msg, Party party);
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
		Party last_attacker;
		OutMessage action;
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE__BATTLE_HPP_
