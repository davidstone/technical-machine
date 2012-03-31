// Connect to Pokemon Lab
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

#ifndef POKEMON_LAB_CONNECT_HPP_
#define POKEMON_LAB_CONNECT_HPP_

#include <cstdint>
#include <string>
#include <vector>
#include <utility>

#include "inmessage.hpp"
#include "../network/connect.hpp"

namespace technicalmachine {
namespace pl {

class Battle;
class Channel;
class Metagame;

class Client : public network::GenericClient {
	private:
		boost::asio::deadline_timer timer;
	public:
		explicit Client (unsigned set_depth);
	private:
		void reset_timer (unsigned timer_length);
		void request_authentication ();
	public:
		void run ();
	private:
		void send_keep_alive_message ();
	public:
		void handle_message (InMessage::Message code, InMessage & msg);
	private:
		void handle_welcome_message (uint32_t version, std::string const & server, std::string const & message) const;
		void handle_password_challenge (InMessage msg);
		enum SecretStyle {
			NONE = 0,
			MD5 = 1,		// This secret style is never actually used.
			VBULLETIN = 2
		};
		std::string get_challenge_response (std::string const & challenge, SecretStyle secret_style, std::string const & salt);
		std::string get_shared_secret (SecretStyle secret_style, std::string const & salt);
		void handle_registry_response (uint8_t code, std::string const & details);
		void join_channel (std::string const & channel);
		void part_channel (std::string const & channel);
		void handle_channel_info (uint32_t channel_id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users);
		void handle_channel_join_part (uint32_t channel_id, std::string const & user, bool joining);
		void handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags);
		void handle_channel_list (std::vector <Channel> const & channel_list);
		void send_battle_challenge (std::string const & opponent);
		void handle_finalize_challenge (std::string const & user, bool accepted, bool challenger);
		void handle_metagame_list (std::vector <Metagame> const & metagames);
		void handle_invalid_team (std::vector <int16_t> const & violations);
		void handle_error_message (uint8_t code, std::string const & details) const;
	public:
		void send_channel_message (uint32_t channel_id, std::string const & message);
	private:
		void send_private_message (std::string const & user, std::string const & message);
};

} // namespace pl
} // namespace technicalmachine
#endif // POKEMON_LAB_CONNECT_HPP_
