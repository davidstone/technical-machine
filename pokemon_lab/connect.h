// Connect to Pokemon Lab
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_LAB_CONNECT_H_
#define POKEMON_LAB_CONNECT_H_

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "../evaluate.h"
#include "battle.h"
#include "inmessage.h"

namespace technicalmachine {
namespace pl {

class Channel;
class Metagame;

class BotClient {
	private:
		std::string username;
		std::string password;
		std::vector <std::string> response;
	public:
		int detailed [END_SPECIES][7];
		Score score;
	private:
		std::map <std::string, Battle> challenges;		// Battles that have not yet begun
		std::map <uint8_t, Battle> battles;			// Battles currently underway
		int depth;
		bool challenger;
	public:
		boost::asio::io_service io;
	private:
		boost::asio::deadline_timer timer;
	public:
		boost::asio::ip::tcp::socket socket;
		BotClient (int depth_);
		void run ();
		void handle_message (InMessage::Message code, InMessage & msg);
		std::string get_response () const;
	private:
		void account_info (std::string & host, std::string & port);
		void connect (std::string const & host, std::string const & port);
		void authenticate ();
		std::string get_shared_secret (int secret_style, std::string const & salt);
		std::string get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt);
		void handle_welcome_message (uint32_t version, std::string const & server, std::string const & message);
		void handle_challenge (InMessage msg);
		void handle_registry_response (uint8_t type, std::string const & details);
		void handle_channel_info (uint32_t id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users);
		void handle_channel_join_part (uint32_t id, std::string const & user, bool joining);
		void handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags);
		void handle_channel_list (std::vector <Channel> const & channels);
		void handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message);
		void join_channel (std::string const & channel);
		void send_battle_challenge ();
		void handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length);
		void handle_finalize_challenge (std::string const & user, bool accepted);
		void handle_battle_begin (uint32_t field_id, std::string const & opponent, uint8_t party);
		void handle_metagame_list (std::vector <Metagame> const & metagames);
		void handle_invalid_team (std::vector <int16_t> const & violation);
		void handle_victory (uint32_t field_id, uint16_t party_id);
		void reset_timer (boost::system::error_code const & error);
		void load_responses ();
};

}
}
#endif
