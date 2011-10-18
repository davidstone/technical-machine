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

#ifndef POKEMON_ONLINE_CONNECT_H_
#define POKEMON_ONLINE_CONNECT_H_

#include <cstdint>
#include <map>
#include <string>

//#include "battle.h"
#include "inmessage.h"
#include "../network/connect.h"

namespace technicalmachine {
namespace po {

class Channel;
class Metagame;

class Client : public network::GenericClient {
	private:
		
//		std::map <std::string, uint32_t> channels;
//		std::map <std::string, Battle> challenges;		// Battles that have not yet begun
//		std::map <uint8_t, Battle> battles;			// Battles currently underway
	public:
		explicit Client (int depth_);
	private:
		void log_in ();
		void authenticate (std::string const & salt);
	public:
		void run ();
	private:
		void send_keep_alive_message ();
	public:
		void handle_message (InMessage::Message code, InMessage & msg);
	private:
		void join_channel (std::string const & channel);
		void part_channel (std::string const & channel);
		void send_battle_challenge (std::string const & opponent);
	public:
		void send_channel_message (std::string channel, std::string const & message);
		void send_channel_message (uint32_t channel_id, std::string const & message);
		void send_private_message (std::string const & user, std::string const & message);
	private:
		void handle_version_control (std::string const & server_version);
		void handle_server_name (std::string const & server_name);
		void handle_announcement (std::string const & announcement);
};

}
}
#endif // POKEMON_ONLINE_CONNECT_HPP_
