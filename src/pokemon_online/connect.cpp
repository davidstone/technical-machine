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

#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include "battle.h"
#include "battle_settings.h"
#include "inmessage.h"
#include "outmessage.h"
#include "read_user_info.h"

#include "../crypt/get_md5.h"
#include "../team.h"

namespace technicalmachine {
namespace po {

Client::Client (int depth_):
	network::GenericClient (depth_),
	team (true, 6) {
	log_in ();
}


void Client::log_in () {
	OutMessage msg (OutMessage::LOG_IN);
	msg.write_team (team, username);
	msg.write_color ();
	bool const ladder_enabled = true;
	msg.write_byte (ladder_enabled);
	bool const show_team = true;
	msg.write_byte (show_team);
	msg.send (*socket);
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

enum ChallengeDescription {
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
		case InMessage::LOG_IN: {
			User const user (msg);
			if (user.name == username)
				my_id = user.id;
			// Should probably throw an exception here
			else
				print_with_time_stamp (std::cerr, "Server claims my username is " + user.name);
			break;
		}
		case InMessage::LOG_OUT: {
			// Only sent to us if we have sent the user a PM.
			uint32_t const user_id = msg.read_int ();
			remove_player (user_id);
			break;
		}
		case InMessage::PLAYERS_LIST: {
			// We get this when a logs in or completes a rated battle.
			User const user (msg);
			add_player (user.id, user.name);
			break;
		}
		case InMessage::SEND_TEAM: {
			// We get this when a user changes their team.
			User const user (msg);
			if (user.id == my_id) {
				// I cannot send a new team if I've been challenged.
				if (challenger)
					send_battle_challenge_with_current_team ();
			}
			break;
		}
		case InMessage::CHALLENGE_STUFF: {
			ChallengeDescription const description = static_cast <ChallengeDescription> (msg.read_byte ());
			uint32_t user_id = msg.read_int ();
			uint32_t const clauses = msg.read_int ();
			uint8_t const mode = msg.read_byte ();
			std::string const & user = get_user_name (user_id);
			switch (description) {
				case SENT: {
					// Received when they challenge me.
					BattleSettings const settings (clauses, mode);
					handle_incoming_challenge (user, settings);
					break;
				}
				default: {
					// Received when I challenge them and they refuse / are unable to battle.
					// ACCEPTED is never sent to me.
					constexpr bool accepted = false;
					handle_finalize_challenge (user, accepted);
					break;
				}
			}
			break;
		}
		case InMessage::BATTLE_BEGIN: {
			uint32_t const battle_id = msg.read_int ();
			uint32_t const user_id1 = msg.read_int ();
			uint32_t const user_id2 = msg.read_int ();
			if (user_id1 == 0) {
				BattleConfiguration configuration (msg);
				// The server then sends me my own team.
				// I don't need to read in my entire team; I already know my own team.
				// I will need to read this if I support Challenge Cup.
				handle_battle_begin (battle_id, get_user_name (user_id2));
			}
			else {
				// Another battle has begun on the server.
			}
			break;
		}
		case InMessage::BATTLE_FINISHED: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const result_code = msg.read_byte ();
			// I completely ignore Close because I determine when I want to close a battle, not my foe.
			// Technical Machine never sends the Close message to allow spectators to talk as long as possible.
			// The server automatically removes the battle when my foe closes it, but that can't be helped.
			if (result_code != 3) {
				uint32_t const winner = msg.read_int ();
				uint32_t const loser = msg.read_int ();
				if (winner == my_id or loser == my_id) {
					Result result = get_result (result_code, winner);
					auto const it = battles.find (battle_id);
					if (it != battles.end ()) {
						Battle & battle = static_cast <Battle &> (*it->second);
						handle_battle_end (battle, battle_id, result);
					}
				}
			}
			break;
		}
		case InMessage::BATTLE_MESSAGE: {
			uint32_t const battle_id = msg.read_int ();
			uint32_t length = msg.read_int ();
			uint8_t const command = msg.read_byte ();
			uint8_t const player = msg.read_byte ();
			length -= (sizeof (command) + sizeof (player));
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_message (*this, battle_id, command, 1 - player, msg);
			}
			break;
		}
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
			print_with_time_stamp (std::cerr, "PLAYER_KICK");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::PLAYER_BAN:
			print_with_time_stamp (std::cerr, "PLAYER_BAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SERV_NUM_CHANGE:
			print_with_time_stamp (std::cerr, "SERV_NUM_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SERV_DESC_CHANGE:
			print_with_time_stamp (std::cerr, "SERV_DESC_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SERV_NAME_CHANGE:
			print_with_time_stamp (std::cerr, "SERV_NAME_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
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
			print_with_time_stamp (std::cerr, "GET_USER_INFO");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::GET_USER_ALIAS:
			print_with_time_stamp (std::cerr, "GET_USER_ALIAS");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::GET_BANLIST:
			print_with_time_stamp (std::cerr, "GET_BANLIST");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::CP_BAN:
			print_with_time_stamp (std::cerr, "CP_BAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::CP_UNBAN:
			print_with_time_stamp (std::cerr, "CP_UNBAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SPECTATE_BATTLE: {
			uint32_t const battle_id = msg.read_int ();
			BattleConfiguration configuration (msg);
			break;
		}
		case InMessage::SPECTATING_BATTLE_MESSAGE:
			print_with_time_stamp (std::cerr, "SPECTATING_BATTLE_MESSAGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::BATTLE_CHAT:
			print_with_time_stamp (std::cerr, "BATTLE_CHAT");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SPECTATING_BATTLE_CHAT:
			print_with_time_stamp (std::cerr, "SPECTATING_BATTLE_CHAT");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SPECTATING_BATTLE_FINISHED:
			print_with_time_stamp (std::cerr, "SPECTATING_BATTLE_FINISHED");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::LADDER_CHANGE:
			print_with_time_stamp (std::cerr, "LADDER_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SHOW_TEAM_CHANGE:
			print_with_time_stamp (std::cerr, "SHOW_TEAM_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::VERSION_CONTROL: {
			std::string const server_version = msg.read_string ();
			handle_version_control (server_version);
			break;
		}
		case InMessage::TIER_SELECTION: {
			uint32_t const bytes_in_tier_list = msg.read_int ();
			// Unfortunately, the tier list contains strings, which can be variable-length.
			// Just ignore this and read until the buffer is empty.
			std::vector <std::pair <uint8_t, std::string>> tiers;
			while (msg.index != msg.buffer.size ()) {
				uint8_t const tier_level = msg.read_byte ();
				std::string const tier_name = msg.read_string ();
				tiers.push_back (std::pair <uint8_t, std::string> (tier_level, tier_name));
			}
			break;
		}
		case InMessage::SERV_MAX_CHANGE:
			print_with_time_stamp (std::cerr, "SERV_MAX_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::FIND_BATTLE:
			print_with_time_stamp (std::cerr, "FIND_BATTLE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::SHOW_RANKINGS:
			print_with_time_stamp (std::cerr, "SHOW_RANKINGS");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::ANNOUNCEMENT: {
			std::string const announcement = msg.read_string ();
			handle_announcement (announcement);
			break;
		}
		case InMessage::CP_TBAN:
			print_with_time_stamp (std::cerr, "CP_TBAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::CP_TUNBAN:
			print_with_time_stamp (std::cerr, "CP_TUNBAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		case InMessage::PLAYER_TBAN: {
			print_with_time_stamp (std::cerr, "PLAYER_TBAN");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::GET_TBAN_LIST: {
			print_with_time_stamp (std::cerr, "GET_TBAN_LIST");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::BATTLE_LIST: {
			// For some reason, PO associates battles with channels
			uint32_t const channel_id = msg.read_int ();
			uint32_t const number_of_battles = msg.read_int ();
			for (unsigned n = 0; n != number_of_battles; ++n)
				add_battle (msg);
			break;
		}
		case InMessage::CHANNELS_LIST: {
			for (uint32_t number = msg.read_int (); number != 0; --number) {
				uint32_t const channel_id = msg.read_int ();
				std::string const channel_name = msg.read_string ();
				handle_add_channel (channel_name, channel_id);
			}
			break;
		}
		case InMessage::CHANNEL_PLAYERS: {
			uint32_t const channel_id = msg.read_int ();
			uint32_t const number_of_players = msg.read_int ();
			std::vector <uint32_t> players;
			players.reserve (number_of_players);
			for (unsigned n = 0; n != number_of_players; ++n)
				players.push_back (msg.read_int ());
			break;
		}
		case InMessage::JOIN_CHANNEL: {
			uint32_t const channel_id = msg.read_int ();
			uint32_t const user_id = msg.read_int ();
			break;
		}
		case InMessage::LEAVE_CHANNEL: {
			uint32_t const channel_id = msg.read_int ();
			uint32_t const user_id = msg.read_int ();
			potentially_remove_player (channel_id, user_id);
			break;
		}
		case InMessage::CHANNEL_BATTLE: {
			print_with_time_stamp (std::cerr, "CHANNEL_BATTLE");
			uint32_t const battle_id = msg.read_int ();
			uint32_t const id1 = msg.read_int ();
			uint32_t const id2 = msg.read_int ();
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::ADD_CHANNEL: {
			std::string const channel_name = msg.read_string ();
			uint32_t const channel_id = msg.read_int ();
			handle_add_channel (channel_name, channel_id);
			break;
		}
		case InMessage::REMOVE_CHANNEL: {
			uint32_t const channel_id = msg.read_int ();
			handle_remove_channel (channel_id);
			break;
		}
		case InMessage::CHAN_NAME_CHANGE: {
			print_with_time_stamp (std::cerr, "CHAN_NAME_CHANGE");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::CHANNEL_MESSAGE: {
			uint32_t const channel_id = msg.read_int ();
			std::string speaker;
			std::string message;
			get_speaker_and_message (msg, speaker, message);
			handle_channel_message (channel_id, speaker, message);
			break;
		}
		case InMessage::SERVER_MESSAGE: {
			std::string speaker;		// by default, "~~Server~~"
			std::string message;
			get_speaker_and_message (msg, speaker, message);
			handle_server_message (speaker, message);
			break;
		}
		case InMessage::HTML_MESSAGE: {
			std::string const message = msg.read_string ();
			// Possibly do some sort of HTML parsing at some point.
			// For now, just print it.
			print_with_time_stamp (std::cerr, message);
			break;
		}
		case InMessage::HTML_CHANNEL: {
			print_with_time_stamp (std::cerr, "HTML_CHANNEL");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::SERVER_NAME: {
			std::string const server_name = msg.read_string ();
			handle_server_name (server_name);
			break;
		}
		case InMessage::SPECIAL_PASS: {
			print_with_time_stamp (std::cerr, "SPECIAL_PASS");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::SERVER_LIST_END: {
			print_with_time_stamp (std::cerr, "SERVER_LIST_END");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		}
		case InMessage::SET_IP:
			print_with_time_stamp (std::cerr, "SET_IP");
			while (msg.index != msg.buffer.size ())
				std::cerr << static_cast <int> (msg.read_byte ()) << '\n';
			break;
		default:
			print_with_time_stamp (std::cerr, "Unknown code: " + boost::lexical_cast<std::string> (code));
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

void Client::add_player (uint32_t user_id, std::string const & user_name) {
	// It's safe to call this on a user that already exists. It will just rewrite the same information.
	user_id_to_name [user_id] = user_name;
	user_name_to_id [user_name] = user_id;
}

void Client::remove_player (uint32_t user_id) {
	// I can get to this code when I leave a channel thanks to my quick fix below.
	if (user_id != my_id) {
		auto const it = user_id_to_name.find (user_id);
		if (it != user_id_to_name.end ()) {
			std::string const user_name = it->second;
			user_id_to_name.erase (it);
			user_name_to_id.erase (user_name);
		}
		else {
			std::string const message = "Server requested removing non-existant player: " + boost::lexical_cast <std::string> (user_id);
			print_with_time_stamp (std::cerr, message);
		}
	}
}

void Client::potentially_remove_player (uint32_t channel_id, uint32_t user_id) {
	// Due to Pokemon Online using user ids instead of user names, I have to keep a map.
	// However, I do not receive a "player logged off" message unless I've sent them a PM.
	// This means that I could create some complicated "hasKnowledgeOf" function like PO,
	// but it would still be wrong and a lot more complicated. For now, I'm assuming all
	// players are in channel 0. TODO: come back and make this slightly more accurate.
	if (channel_id == 0)
		remove_player (user_id);
}

void Client::send_battle_challenge (std::string const & opponent) {
	// Due to Pokemon Online's team registration feature, I only support one queued challenge at a time.
	// This seems to be a better option than removing the ability to randomly select a team.
	// I could support multiple challenges and mostly randomized teams, but that would be much more work.
	// It would require using a queue instead of a map.
	if (challenges.empty () and get_user_id (opponent)) {
		std::shared_ptr <Battle> battle (new Battle (opponent, depth));
		add_pending_challenge (battle);
		challenger = true;
		OutMessage msg (OutMessage::SEND_TEAM);
		msg.write_team (battle->ai, username);
		msg.send (*socket);
	}
}

void Client::send_battle_challenge_with_current_team () {
	if (!challenges.empty ()) {
		OutMessage msg (OutMessage::CHALLENGE_STUFF);
		uint32_t const user_id = get_user_id (challenges.begin ()->first);
		uint8_t const generation = 4;		// ???
		BattleSettings const settings (BattleSettings::SPECIES_CLAUSE, BattleSettings::SINGLES);
		msg.write_challenge (user_id, generation, settings);
		msg.send (*socket);
	}
}

void Client::handle_finalize_challenge (std::string const & opponent, bool accepted, bool unused) {
	OutMessage msg (OutMessage::CHALLENGE_STUFF);
	std::string verb;
	// See the description of send_battle_challenge() for why I make sure challenges is empty
	if (accepted and challenges.empty ()) {
		// They challenged me.
		std::shared_ptr <Battle> battle (new Battle (opponent, depth, team));
		add_pending_challenge (battle);
		msg.write_byte (ACCEPTED);
		challenger = false;
		verb = "Accepted";
	}
	else {
		if (!challenges.empty ())
			challenges.erase (challenges.begin ());
		msg.write_byte (REFUSED);
		verb = "Rejected";
	}
	msg.write_int (get_user_id (opponent));
	msg.send (*socket);
	print_with_time_stamp (std::cout, verb + " challenge vs. " + opponent);
}

void Client::handle_remove_challenge (std::string const & opponent) {
}

void Client::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	msg.send (*socket);
}

void Client::part_channel (std::string const & channel) {
	auto const it = channels.find (channel);
	if (it != channels.end ()) {
		OutMessage msg (OutMessage::LEAVE_CHANNEL);
		msg.write_int (it->second);
		msg.send (*socket);
	}
}

void Client::handle_add_channel (std::string const & channel_name, uint32_t channel_id) {
	channels.insert (std::pair <std::string, uint32_t> (channel_name, channel_id));
	id_to_channel.insert (std::pair <uint32_t, std::string> (channel_id, channel_name));
}

void Client::handle_remove_channel (uint32_t channel_id) {
	auto const it = id_to_channel.find (channel_id);
	if (it != id_to_channel.end()) {
		channels.erase (it->second);
		id_to_channel.erase (it);
	}
}

void Client::add_battle (InMessage & msg) {
	// Not sure if any of this is relevant. If it is, all_battles will be added to the class.
	std::multimap <uint32_t, uint32_t> all_battles;
	uint32_t const battle_id = msg.read_int ();
	uint32_t const player1 = msg.read_int ();
	uint32_t const player2 = msg.read_int ();
	std::pair <uint32_t, uint32_t> const match1 (player1, battle_id);
	all_battles.insert (match1);
	std::pair <uint32_t, uint32_t> const match2 (player2, battle_id);
	all_battles.insert (match2);
}

void Client::remove_battle (InMessage & msg) {
}

void Client::send_channel_message (uint32_t channel_id, std::string const & message) {
	OutMessage msg (OutMessage::CHANNEL_MESSAGE);
	msg.write_int (channel_id);
	msg.write_string (message);
	msg.send (*socket);
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
	if (version != server_version) {
		print_with_time_stamp (std::cerr, "Server version is: " + server_version);
		print_with_time_stamp (std::cerr, "User's 'version': " + version);
	}
}

void Client::handle_server_name (std::string const & server_name) const {
	print_with_time_stamp (std::cout, "Server name: " + server_name);
}

void Client::handle_announcement (std::string const & announcement) const {
	print_with_time_stamp (std::cout, announcement);
}

void Client::handle_private_message (uint32_t user_id, std::string const & message) {
	std::string const user = get_user_name (user_id);
	GenericClient::handle_private_message (user, message);
}

uint32_t Client::get_user_id (std::string const & name) const {
	auto const it = user_name_to_id.find (name);
	if (it != user_name_to_id.end ())
		return it->second;
	else {
		print_with_time_stamp (std::cerr, "User name " + name + " not in user_name_to_id.");
		return 0;
	}
}

std::string Client::get_user_name (uint32_t id) const {
	auto const it = user_id_to_name.find (id);
	if (it != user_id_to_name.end ())
		return it->second;
	else {
		print_with_time_stamp (std::cerr, "User id " + std::to_string (id) + " not in user_id_to_name.");
		return "";
	}
}

network::GenericClient::Result Client::get_result (uint8_t code, uint32_t winner) const {
	// I completely ignore Close because I determine when I want to close a battle, not my foe.
	// I also ignore Forfeit. Technical Machine never forfeits, and if the foe forfeits, then it is a win.
	Result result;
	if (code != 2)
		result = (winner == my_id) ? WON : LOST;
	else
		result = TIED;
	return result;
}

} // namespace po
} // namespace technicalmachine
