// Connect to Pokemon Online
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

#ifndef POKEMON_ONLINE__CONNECT_HPP_
#define POKEMON_ONLINE__CONNECT_HPP_

#include <cstdint>
#include <map>
#include <string>

#include "inmessage.hpp"
#include "../network/client.hpp"
#include "../../team.hpp"

namespace technicalmachine {
namespace po {

class Clauses;

class Client : public network::GenericClient {
	public:
		explicit Client (unsigned set_depth);
		void run ();
		void handle_message (InMessage::Message code, InMessage & msg);
		void send_channel_message (std::string channel, std::string const & message);
		void send_channel_message(std::string const & channel, std::string const & message);
		void send_channel_message (uint32_t channel_id, std::string const & message);
		void send_private_message (std::string const & user, std::string const & message);
		void send_private_message (uint32_t user_id, std::string const & message);
		std::string get_user_name (uint32_t id) const;
		void write_team();
	private:
		void handle_log_in (InMessage & msg);
		void handle_log_out (InMessage & msg);
		void handle_players_list (InMessage & msg);
		void handle_send_team (InMessage & msg);
		void handle_challenge_stuff (InMessage & msg);
		void parse_battle_begin (InMessage & msg);
		void parse_battle_finished (InMessage & msg);
		void handle_battle_message (InMessage & msg);
		void parse_ask_for_pass (InMessage & msg);
		void handle_player_kick (InMessage & msg) const;
		void handle_player_ban (InMessage & msg) const;
		void handle_serv_num_change (InMessage & msg) const;
		void handle_serv_desc_change (InMessage & msg) const;
		void handle_serv_name_change (InMessage & msg) const;
		void parse_private_message (InMessage & msg);
		void handle_get_user_info (InMessage & msg) const;
		void handle_get_user_alias (InMessage & msg) const;
		void handle_get_banlist (InMessage & msg) const;
		void handle_cp_ban (InMessage & msg) const;
		void handle_cp_unban (InMessage & msg) const;
		void handle_spectate_battle (InMessage & msg) const;
		void handle_spectating_battle_message (InMessage & msg) const;
		void handle_battle_chat (InMessage & msg) const;
		void handle_spectating_battle_chat (InMessage & msg) const;
		void handle_spectating_battle_finished (InMessage & msg) const;
		void handle_show_team_change (InMessage & msg) const;
		void handle_ladder_change (InMessage & msg) const;
		void parse_version_control (InMessage & msg) const;
		void handle_tier_selection (InMessage & msg) const;
		void handle_serv_max_change (InMessage & msg) const;
		void handle_find_battle (InMessage & msg) const;
		void handle_show_rankings (InMessage & msg) const;
		void parse_announcement (InMessage & msg) const;
		void handle_cp_tban (InMessage & msg) const;
		void handle_cp_tunban (InMessage & msg) const;
		void handle_player_tban (InMessage & msg) const;
		void handle_get_tban_list (InMessage & msg) const;
		void handle_battle_list (InMessage & msg);
		void handle_channels_list (InMessage & msg);
		void handle_channel_players (InMessage & msg);
		void handle_join_channel (InMessage & msg);
		void handle_leave_channel (InMessage & msg);
		void handle_channel_battle (InMessage & msg) const;
		void parse_add_channel (InMessage & msg);
		void parse_remove_channel (InMessage & msg);
		void handle_channel_name_change (InMessage & msg) const;
		void parse_channel_message (InMessage & msg) const;
		void parse_server_message (InMessage & msg) const;
		void handle_html_message (InMessage & msg) const;
		void handle_html_channel (InMessage & msg) const;
		void parse_server_name (InMessage & msg) const;
		void handle_special_pass (InMessage & msg) const;
		void handle_server_list_end (InMessage & msg) const;
		void handle_set_ip (InMessage & msg) const;
		void handle_unknown_message_code (InMessage::Message code) const;
		void handle_unimplemented_message (InMessage & msg, std::string const & message_name) const;
		void log_in ();
		void authenticate (std::string const & salt);
		void add_player (uint32_t user_id, std::string const & user_string);
		void remove_player (uint32_t user_id);
		void potentially_remove_player (uint32_t channel_id, uint32_t user_id);

		// Must return to reading messages before assuming the battle has begun
		// Waits until it receives confirmation that the server has the new team to actually challenge.
		void send_battle_challenge (std::string const & opponent);
		void send_battle_challenge_with_current_team ();

		void handle_finalize_challenge (std::string const & opponent, bool accepted, bool unused = false);
		void handle_remove_challenge (std::string const & opponent);
		void send_keep_alive_message ();
		void join_channel (std::string const & channel);
		void part_channel (std::string const & channel);
		void handle_add_channel (std::string const & channel_name, uint32_t channel_id);
		void handle_remove_channel (uint32_t channel_id);
		void add_battle (InMessage & msg);
		void remove_battle (InMessage & msg);
		void send_registration_message ();
		void handle_version_control (std::string const & server_version) const;
		void handle_server_name (std::string const & server_name) const;
		void handle_announcement (std::string const & announcement) const;
		void handle_private_message (uint32_t user_id, std::string const & message);
		uint32_t get_user_id (std::string const & name) const;
		Result get_result (uint8_t code, uint32_t winner) const;

	public:
		// I cannot pick a new team when challenged. I reload this at the start
		// of a new battle to prevent counter-teaming. It's only used when challenged.
		Team team;
	private:
		std::map <uint32_t, std::string> user_id_to_name;
		std::map <std::string, uint32_t> user_name_to_id;
		std::map <std::string, uint32_t> channel_to_id;
		std::map <uint32_t, std::string> id_to_channel;
		uint32_t my_id;
};

}	// namespace po
}	// namespace technicalmachine
#endif	// POKEMON_ONLINE__CONNECT_HPP_
