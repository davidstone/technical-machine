// Connect to Pokemon Online
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "connect.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>

#include <boost/algorithm/string.hpp>

#include "inmessage.h"
#include "outmessage.h"

#include "../crypt/get_md5.h"

#include <iostream>

namespace technicalmachine {
namespace po {

Client::Client (int depth_) : network::GenericClient (depth_) {
	log_in ();
}

void Client::log_in () {
	OutMessage message (OutMessage::LOG_IN);
	message.write_string (username);
	message.send (*socket);
}

void Client::run () {
	InMessage msg;
	msg.read_header (*socket, this);
	io.run();
}

void Client::send_keep_alive_message () {
	OutMessage msg (OutMessage::KEEP_ALIVE);
	msg.send (*socket);
}

void get_speaker_and_message (InMessage & msg, std::string & speaker, std::string & message) {
	// The server sends one string of the form "speaker: message" rather than sending the speaker and the message separately.
	std::string const speaker_and_message = msg.read_string ();
	std::string const delimiter = ": ";
	size_t delimiter_position = speaker_and_message.find (delimiter);
	speaker = speaker_and_message.substr (0, delimiter_position);
	message = speaker_and_message.substr (delimiter_position + delimiter.size());
}

void Client::handle_message (InMessage::Message code, InMessage & msg) {
	switch (code) {
		case InMessage::WHAT_ARE_YOU:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::WHO_ARE_YOU:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::LOG_IN:
			std::cerr << "LOG_IN\n";
			break;
		case InMessage::LOG_OUT:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SEND_MESSAGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::PLAYERS_LIST: {
			// The server sends this message whenever a whole bunch of stuff about a user changes or when they log in. This message is sent if a user's rating changes, for instance, so we get it at the end of the every battle.
			uint32_t const user_id = msg.read_int ();
			std::string const user_string = msg.read_string ();
			
			// If the user does not have a team loaded at all, it seems as though this can lead to an invalid read. We don't really care about this stuff right now, so I'll just ignore it.
//			std::string const user_message = msg.read_string ();
//			int8_t auth_level = msg.read_byte ();
//			uint8_t flags = msg.read_byte ();
//			bool logged_in = false;
//			bool battling = false;
//			bool away = false;
//			if (flags % 2 >= 1)
//				logged_in = true;
//			if (flags % 4 >= 2)
//				battling = true;
//			if (flags % 8 >= 4)
//				away = true;
//			int16_t rating = msg.read_short ();
			// This should help if a user leaves and rejoins the server with a different user ID
			user_id_to_name [user_id] = user_string;
			user_name_to_id [user_string] = user_id;
			break;
		}
		case InMessage::SEND_TEAM: {
			std::cerr << "SEND_TEAM size: " << msg.buffer.size () << '\n';
			break;
		}
		case InMessage::CHALLENGE_STUFF: {
			uint8_t byte = msg.read_byte ();
			uint32_t user_id = msg.read_int ();
			std::string const & user = user_id_to_name.find (user_id)->second;
			std::cerr << user + '\n';
			for (std::vector<uint8_t>::const_iterator it = msg.buffer.begin() + msg.index; it != msg.buffer.end (); ++it)
				std::cerr << static_cast <int> (*it) << '\n';
			break;
		}
		case InMessage::ENGAGE_BATTLE:
			// A battle started on the server.
			break;
		case InMessage::BATTLE_FINISHED:
			// A battle finished on the server.
			break;
		case InMessage::BATTLE_MESSAGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::BATTLE_CHAT:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::KEEP_ALIVE:
			// This currently doesn't do anything on the server...
			send_keep_alive_message ();
			break;
		case InMessage::ASK_FOR_PASS: {
			std::string const & salt = msg.read_string ();
			authenticate (salt);
			break;
		}
		case InMessage::REGISTER: {
			// Message telling me that I am not registered.
			break;
		}
		case InMessage::PLAYER_KICK:
			break;
		case InMessage::PLAYER_BAN:
			break;
		case InMessage::SERV_NUM_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SERV_DESC_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SERV_NAME_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SEND_PM: {
			uint32_t const user_id = msg.read_int ();
			std::string const message = msg.read_string ();
			handle_private_message (user_id, message);
			break;
		}
		case InMessage::AWAY:
			// Someone has gone away.
			break;
		case InMessage::GET_USER_INFO:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::GET_USER_ALIAS:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::GET_BANLIST:
			break;
		case InMessage::CP_BAN:
			break;
		case InMessage::CP_UNBAN:
			break;
		case InMessage::SPECTATE_BATTLE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SPECTATING_BATTLE_MESSAGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SPECTATING_BATTLE_CHAT:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SPECTATING_BATTLE_FINISHED:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::LADDER_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SHOW_TEAM_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::VERSION_CONTROL: {
			std::string const server_version = msg.read_string ();
			handle_version_control (server_version);
			break;
		}
		case InMessage::TIER_SELECTION:
			break;
		case InMessage::SERV_MAX_CHANGE:
			break;
		case InMessage::FIND_BATTLE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SHOW_RANKINGS:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::ANNOUNCEMENT: {
			std::string const announcement = msg.read_string ();
			handle_announcement (announcement);
			break;
		}
		case InMessage::CP_TBAN:
			break;
		case InMessage::CP_TUNBAN:
			break;
		case InMessage::PLAYER_TBAN:
			break;
		case InMessage::GET_TBAN_LIST:
			break;
		case InMessage::BATTLE_LIST:
			// All battles currently in progress.
			break;
		case InMessage::CHANNELS_LIST:
			// List of all channels on the server.
			break;
		case InMessage::CHANNEL_PLAYERS:
			// List of all people on each channel
			break;
		case InMessage::JOIN_CHANNEL:
			// Someone has joined the channel
			break;
		case InMessage::LEAVE_CHANNEL:
			// Someone has left the channel
			break;
		case InMessage::CHANNEL_BATTLE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::REMOVE_CHANNEL:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::ADD_CHANNEL:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::CHANNEL_MESSAGE: {
			uint32_t const channel_id = msg.read_int ();
			std::string speaker;
			std::string message;
			get_speaker_and_message (msg, speaker, message);
			handle_channel_message (channel_id, speaker, message);
			break;
		}
		case InMessage::CHAN_NAME_CHANGE:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::HTML_MESSAGE:
			break;
		case InMessage::HTML_CHANNEL:
			break;
		case InMessage::SERVER_NAME: {
			std::string const server_name = msg.read_string ();
			handle_server_name (server_name);
			break;
		}
		case InMessage::SPECIAL_PASS:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SERVER_LIST_END:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SET_IP:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		default:
			std::cerr << "Unknown code: " << code << '\n';
			break;
	}
	msg.read_header (*socket, this);
}

void Client::authenticate (std::string const & salt) {
	OutMessage msg (OutMessage::ASK_FOR_PASS);
	// Pokemon Online bases its hash on lowercase a-f.
	std::string hash = getMD5HexHash (password);
	boost::algorithm::to_lower (hash);
	hash = getMD5HexHash (hash + salt);
	boost::algorithm::to_lower (hash);
	msg.write_string (hash);
	msg.send (*socket);
}
 
void Client::join_channel (std::string const & channel) {
}

void Client::part_channel (std::string const & channel) {
}

void Client::send_battle_challenge (std::string const & opponent) {
}

void Client::send_channel_message (std::string channel, std::string const & message) {
}

void Client::send_channel_message (uint32_t channel_id, std::string const & message) {
}

void Client::send_private_message (std::string const & user, std::string const & message) {
	send_private_message (user_name_to_id.find (user)->second, message);
}

void Client::send_private_message (uint32_t user_id, std::string const & message) {
	OutMessage msg (OutMessage::SEND_PM);
	msg.write_int (user_id);
	msg.write_string (message);
	msg.send (*socket);
}

void Client::handle_version_control (std::string const & server_version) {
//	OutMessage msg (OutMessage::VERSION_CONTROL);
	// Pretend to be the most recent version because this is the standard I'm coding against.
	std::string const version = "1.0.30";
	if (version != server_version)
		std::cerr << "Server version is: " + server_version + "\nUser's 'version': " + version + "\n";
}

void Client::handle_server_name (std::string const & server_name) {
	std::cout << "Server name: " + server_name + '\n';
}

void Client::handle_announcement (std::string const & announcement) {
	std::cout << announcement + '\n';
}

void Client::handle_private_message (uint32_t user_id, std::string const & message) {
	std::string const user = user_id_to_name.find (user_id)->second;
	GenericClient::handle_private_message (user, message);
}

} // namespace po
} // namespace technicalmachine
