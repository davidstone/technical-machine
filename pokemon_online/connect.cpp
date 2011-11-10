// Connect to Pokemon Online
// Copyright (C) 2011 David Stone
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

#include "connect.h"

#include <cstdint>
#include <map>
#include <string>
#include <utility>

#include <boost/algorithm/string.hpp>

#include "battle.h"
#include "battle_settings.h"
#include "inmessage.h"
#include "outmessage.h"

#include "../crypt/get_md5.h"
#include "../team.h"

#include <iostream>

namespace technicalmachine {
namespace po {

Client::Client (int depth_) : network::GenericClient (depth_) {
	log_in ();
}


void Client::log_in () {
	OutMessage message (OutMessage::LOG_IN);
	Team ai (true, 6);
	message.write_team (ai, username);
	
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

enum Challenge_Description {
	SENT = 0,
	ACCEPTED = 1,
	CANCELED = 2,
	BUSY = 3,
	REFUSED = 4,
	INVALID_TEAM = 5,
	DIFFERENT_GENERATION = 6
};

enum Action {
	CANCEL = 0,
	ATTACK = 1,
	SWITCH = 2,
	REARRANGE = 3,
	CENTER_MOVE = 4,
	DRAW = 5
};

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
		case InMessage::LOG_IN: {
			uint32_t const player_id = msg.read_int ();
			std::string const player_name = msg.read_string ();
			std::string const info = msg.read_string ();
			int8_t const authority = msg.read_byte ();
			uint8_t const flags = msg.read_byte ();
			int16_t rating = msg.read_short ();
			std::vector <std::pair <uint16_t, uint8_t> > team;
			team.reserve (6);
			for (unsigned n = 0; n != 6; ++n) {
				uint16_t const species = msg.read_short ();
				uint8_t forme = msg.read_byte ();
				team.push_back (std::pair <uint16_t, uint8_t> (species, forme));
			}
			uint16_t avatar = msg.read_short ();
			std::string const tier = msg.read_string ();
			uint8_t const color_spec = msg.read_byte ();
			uint16_t const alpha = msg.read_short ();
			uint16_t const red = msg.read_short ();
			uint16_t const green = msg.read_short ();
			uint16_t const blue = msg.read_short ();
			uint16_t const pad = msg.read_short ();
			uint8_t gen = msg.read_byte ();
			break;
		}
		case InMessage::LOG_OUT:
			std::cerr << "code: " << code << '\n';
			std::cerr << "size: " << msg.buffer.size() << '\n';
			break;
		case InMessage::SEND_MESSAGE: {
			std::string const message = msg.read_string ();
			print_with_time_stamp (message);
			break;
		}
		case InMessage::PLAYERS_LIST: {
			// The server sends this message whenever a whole bunch of stuff about a user changes or when they log in. This message is sent if a user's rating changes, for instance, so we get it at the end of the every rated battle.
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
			Challenge_Description const description = static_cast <Challenge_Description> (msg.read_byte ());
			uint32_t user_id = msg.read_int ();
			std::string const & user = get_user_name (user_id);
			uint32_t const clauses = msg.read_int ();
			uint8_t const mode = msg.read_byte ();
			BattleSettings const settings (clauses, mode);
			switch (description) {
				case SENT:
					handle_incoming_challenge (user, settings);
					break;
				case ACCEPTED: {
					bool const accepted = true;
					bool const challenger = true;
					handle_finalize_challenge (user, accepted, challenger);
					break;
				}
				default: {
					bool const accepted = false;
					bool const challenger = true;
					handle_finalize_challenge (user, accepted, challenger);
					break;
				}
			}
			break;
		}
		case InMessage::ENGAGE_BATTLE: {
			uint32_t const battle_id = msg.read_int ();
			uint32_t const id1 = msg.read_int ();
			uint32_t const id2 = msg.read_int ();
			// Currently only care about battles I'm in. TM isn't a spectator.
			if (id1 == 0) {
				// I have no idea what these two are actually for.
				uint32_t const id_first = msg.read_int ();
				uint32_t const id_second = msg.read_int ();
				// The server then sends me my own team.
				// I don't need to read in my entire team; I already know my own team.
				handle_battle_begin (battle_id, get_user_name (id2));
			}
			break;
		}
		case InMessage::BATTLE_FINISHED: {
			std::cerr << "BATTLE_FINISHED\n";
			uint32_t const battle_id = msg.read_int ();
			int8_t const description = msg.read_byte ();
			std::cerr << "description: " << static_cast <int> (description) << '\n';
			uint32_t const id1 = msg.read_int ();
			std::cerr << "id1: " << static_cast <int> (id1) << '\n';
			uint32_t const id2 = msg.read_int ();
			std::cerr << "id2: " << static_cast <int> (id2) << '\n';
			break;
		}
		case InMessage::BATTLE_MESSAGE: {
			uint32_t const battle_id = msg.read_int ();
			uint32_t length = msg.read_int ();
			uint8_t const command = msg.read_byte ();
			uint8_t const player = msg.read_byte ();
			length -= sizeof (command) + sizeof (player);
			Battle & battle = static_cast <Battle &> (*battles.find (battle_id)->second);
			battle.handle_message (*this, battle_id, command, 1 - player, msg);
			break;
		}
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

void Client::handle_finalize_challenge (std::string const & opponent, bool accepted, bool challenger) {
	OutMessage msg (OutMessage::CHALLENGE_STUFF);
	// If I am the challenger, I don't write the accepted byte.
	if (!challenger)
		msg.write_byte (accepted ? ACCEPTED : REFUSED);
	msg.write_int (get_user_id (opponent));
	std::string verb;
	if (accepted) {
		std::shared_ptr <Battle> battle (new Battle (opponent, depth));
		add_pending_challenge (battle);
		verb = "Accepted";
	}
	else
		verb = "Rejected";
	msg.send (*socket);
	print_with_time_stamp (verb + " challenge vs. " + opponent);
}

void Client::handle_remove_challenge (std::string const & opponent) {
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
	send_private_message (get_user_id (user), message);
}

void Client::send_private_message (uint32_t user_id, std::string const & message) {
	OutMessage msg (OutMessage::SEND_PM);
	msg.write_int (user_id);
	msg.write_string (message);
	msg.send (*socket);
}

void Client::handle_version_control (std::string const & server_version) const {
//	OutMessage msg (OutMessage::VERSION_CONTROL);
	// Pretend to be the most recent version because this is the standard I'm coding against.
	std::string const version = "1.0.30";
	if (version != server_version)
		std::cerr << "Server version is: " + server_version + "\nUser's 'version': " + version + "\n";
}

void Client::handle_server_name (std::string const & server_name) const {
	std::cout << "Server name: " + server_name + '\n';
}

void Client::handle_announcement (std::string const & announcement) const {
	std::cout << announcement + '\n';
}

void Client::handle_private_message (uint32_t user_id, std::string const & message) {
	std::string const user = get_user_name (user_id);
	GenericClient::handle_private_message (user, message);
}

uint32_t Client::get_user_id (std::string const & name) const {
	return user_name_to_id.find (name)->second;
}

std::string Client::get_user_name (uint32_t id) const {
	return user_id_to_name.find (id)->second;
}

} // namespace po
} // namespace technicalmachine
