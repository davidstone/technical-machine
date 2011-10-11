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

#include "connect.h"

#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "battle.h"
#include "inmessage.h"
#include "outmessage.h"

#include "../crypt/get_md5.h"
#include "../crypt/get_sha2.h"
#include "../crypt/rijndael.h"

namespace technicalmachine {
namespace pl {

BotClient::BotClient (int depth_) : network::GenericClient (depth_) {
	request_authentication ();
}

void BotClient::request_authentication () {
	OutMessage message (OutMessage::REQUEST_CHALLENGE);
	message.write_string (username);
	message.send (socket);
}

void BotClient::run () {
	unsigned const timer_length = 45;
	reset_timer (timer_length);

	InMessage msg;
	msg.read_header (socket, this);

	io.run();
}

void BotClient::send_keep_alive_message () {
	OutMessage msg (OutMessage::CLIENT_ACTIVITY);
	msg.send (socket);
}

class Channel {
	public:
		std::string name;
		uint8_t type;
		std::string topic;
		uint32_t population;
		Channel (InMessage & msg):
			name (msg.read_string()),
			type (msg.read_byte()),
			topic (msg.read_string()),
			population (msg.read_int()) {
		}
};

class Metagame {
	public:
		uint8_t index;
		std::string name;
		std::string id;
		std::string description;
		uint8_t party_size;
		uint8_t max_team_length;

		std::vector <uint16_t> bans;
		std::vector <std::string> clauses;

		bool timing;
		uint16_t pool_length;
		uint8_t periods;
		uint16_t period_length;
	private:
		void load_bans (InMessage & msg) {
			uint16_t ban_list_count = msg.read_short ();
			for (uint16_t n = 0; n != ban_list_count; ++n) {
				uint16_t species_id = msg.read_short ();
				bans.push_back (species_id);
			}
		}
		void load_clauses (InMessage & msg) {
			uint16_t clause_count = msg.read_short ();
			for (uint16_t b = 0; b != clause_count; ++b) {
				std::string clause_name = msg.read_string ();
				clauses.push_back (clause_name);
			}
		}
		void load_battle_timer (InMessage & msg) {
			timing = msg.read_byte ();
			if (timing) {
				pool_length = msg.read_short ();
				periods = msg.read_byte ();
				period_length = msg.read_short ();
			}
		}
	public:
		Metagame (InMessage & msg):
			index (msg.read_byte ()),
			name (msg.read_string ()),
			id (msg.read_string ()),
			description (msg.read_string ()),
			party_size (msg.read_byte ()),
			max_team_length (msg.read_byte ()),
			pool_length (-1),
			periods (-1),
			period_length (-1) {
			load_bans (msg);
			load_clauses (msg);
			load_battle_timer (msg);
		}
};

void BotClient::handle_message (InMessage::Message code, InMessage & msg) {
	switch (code) {
		case InMessage::WELCOME_MESSAGE: {
			int32_t const version = msg.read_int();
			std::string const server_name = msg.read_string();
			std::string const message = msg.read_string();
//			 Apparently these are not needed.
//			uint8_t const registrations = msg.read_byte ();
//			std::string const login_info = msg.read_string ();
//			std::string const registration_info = msg.read_string ();
			handle_welcome_message (version, server_name, message);
			break;
		}
		case InMessage::PASSWORD_CHALLENGE:
			handle_password_challenge (msg);
			break;
		case InMessage::REGISTRY_RESPONSE: {
			uint8_t const type = msg.read_byte();
			std::string const details = msg.read_string();
			handle_registry_response (type, details);
			break;
		}
		case InMessage::SERVER_INFO:
			// Apparently unused
			break;
		case InMessage::CHANNEL_INFO: {
			uint32_t const id = msg.read_int();
			uint8_t const info = msg.read_byte();
			std::string const channel_name = msg.read_string();
			std::string const topic = msg.read_string();
			uint32_t const channel_flags = msg.read_int();
			uint32_t const number_of_users = msg.read_int();
			std::vector <std::pair <std::string, uint32_t> > users;
			for (uint32_t n = 0; n != number_of_users; ++n) {
				std::string const username = msg.read_string();
				uint32_t const flags = msg.read_int();
				std::pair <std::string, uint32_t> const user (username, flags);
				users.push_back (user);
			}
			handle_channel_info (id, info, channel_name, topic, channel_flags, users);
			break;
		}
		case InMessage::CHANNEL_JOIN_PART: {
/*			uint32_t const id = msg.read_int();
			std::string const user = msg.read_string();
			bool const joining = msg.read_byte();
			handle_channel_join_part (id, user, joining);
*/			break;
		}
		case InMessage::CHANNEL_STATUS: {
/*			uint32_t const channel_id = msg.read_int();
			std::string const invoker = msg.read_string();
			std::string const user = msg.read_string();
			uint32_t const flags = msg.read_int();
			handle_channel_status (channel_id, invoker, user, flags);
*/			break;
		}
		case InMessage::CHANNEL_LIST: {
/*			uint32_t const number_of_channels = msg.read_int();
			std::vector <Channel> channels;
			for (uint32_t n = 0; n != number_of_channels; ++n) {
				Channel channel (msg);
				channels.push_back (channel);
			}
			handle_channel_list (channels);
*/			break;
		}
		case InMessage::CHANNEL_MESSAGE: {
			uint32_t const channel_id = msg.read_int();
			std::string const user = msg.read_string ();
			std::string const message = msg.read_string ();
			handle_channel_message (channel_id, user, message);
			break;
		}
		case InMessage::INCOMING_CHALLENGE: {
			std::string const user = msg.read_string ();
			uint8_t const generation = msg.read_byte ();
			uint32_t const active_party_size = msg.read_int ();
			uint32_t const max_team_length = msg.read_int ();
/*			int32_t const metagame = msg.read_int ();
			std::vector <uint8_t> clauses;
			bool timed;
			int32_t pool;
			uint8_t periods;
			int32_t period_length;
			if (metagame == -1) {
				uint8_t const clauses_size = msg.read_byte ();
				for (uint8_t n = 0; n != clauses.size(); ++n) {
					uint8_t clause = msg.read_byte ();
					clauses.push_back (clause);
				}
				timed = msg.read_byte ();
				if (timed) {
					pool = msg.read_int ();
					periods = msg.read_byte ();
					period_length = msg.read_int ();
				}
			}
*/			handle_incoming_challenge (user, generation, active_party_size, max_team_length);
			break;
		}
		case InMessage::FINALIZE_CHALLENGE: {
			std::string const user = msg.read_string ();
			bool const accepted = msg.read_byte ();
			bool challenger = true;
			handle_finalize_challenge (user, accepted, challenger);
			break;
		}
		case InMessage::CHALLENGE_WITHDRAWN: {
			std::string const opponent = msg.read_string ();
			challenges.erase (opponent);
		}
		case InMessage::BATTLE_BEGIN: {
			uint32_t const field_id = msg.read_int ();
			std::string const opponent = msg.read_string ();
			uint8_t const party = msg.read_byte ();
/*			int16_t const metagame = msg.read_short ();
			bool const rated = msg.read_byte ();
			std::string const battle_id = msg.read_string ();
*/			handle_battle_begin (field_id, opponent, party);
			break;
		}
		case InMessage::REQUEST_ACTION: {
			uint32_t const field_id = msg.read_int ();
			uint8_t  const slot = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			bool const replace = msg.read_byte ();
			uint8_t const request_sequences = msg.read_byte ();
			uint8_t const sequential_requests = msg.read_byte ();
			uint32_t const number_of_pokemon = msg.read_int ();
			// Avoiding the vector <bool> template specialization
			std::vector <uint8_t> switches;
			for (uint32_t n = 0; n != number_of_pokemon; ++n) {
				// Whether it's legal to switch to this Pokemon
				bool can_switch_to = msg.read_byte ();
				switches.push_back (can_switch_to);
			}

			bool can_switch = false;
			bool forced = false;
			std::vector <uint8_t> moves;
			if (!replace) {
				can_switch = msg.read_byte ();
				forced = msg.read_byte ();
				if (!forced) {
					uint8_t num_moves = msg.read_int ();
					for (uint8_t n = 0; n != num_moves; ++n)
						moves.push_back (msg.read_byte ());
				}
			}
			bool can_switch_to = false;
			for (std::vector <uint8_t>::const_iterator it = switches.begin(); it != switches.end(); ++it) {
				if (*it) {
					can_switch_to = true;
					break;
				}
			}
			can_switch = can_switch_to;
			Battle & battle = battles.find (field_id)->second;
			battle.handle_request_action (*this, field_id, slot, index, replace, switches, can_switch, forced, moves);
			break;
		}
		case InMessage::BATTLE_POKEMON: {
			// int32 : field id
			// for 0..1:
			//	 for 0..n-1:
			//		 int16 : species id
			//		 if id != -1:
			//			 byte: gender
			//			 byte: level
			//			 byte: shiny
		   
			// This is data related to visual stuff... Not needed for the bot.
			break;
		}
		case InMessage::BATTLE_PRINT: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const category = msg.read_byte ();
			int16_t const message_id = msg.read_short ();
			uint8_t argc = msg.read_byte ();
			std::vector <std::string> arguments;
			for (uint8_t n = 0; n != argc; ++n) {
				std::string const argument = msg.read_string ();
				arguments.push_back (argument);
			}
			Battle & battle = battles.find (field_id)->second;
			battle.handle_print (category, message_id, arguments);
			break;
		}
		case InMessage::BATTLE_VICTORY: {
			uint32_t const field_id = msg.read_int ();
			int16_t const party_id = msg.read_short ();
			handle_victory (field_id, party_id);
			break;
		}
		case InMessage::BATTLE_USE_MOVE: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			int16_t const move_id = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_use_move (party, slot, nickname, move_id);
			break;
		}
		case InMessage::BATTLE_WITHDRAW: {
/*			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_withdraw (party, slot, nickname);
*/			break;
		}
		case InMessage::BATTLE_SEND_OUT: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			int16_t const species_id = msg.read_short ();
			int8_t const gender = msg.read_byte ();
			uint8_t const level = msg.read_byte();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_send_out (party, slot, index, nickname, species_id, gender, level);
			break;
		}
		case InMessage::BATTLE_HEALTH_CHANGE: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			int16_t const change_in_health = msg.read_short ();
			int16_t const remaining_health = msg.read_short ();
			int16_t const denominator = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_health_change (party, slot, change_in_health, remaining_health, denominator);
			break;
		}
		case InMessage::BATTLE_SET_PP: {
/*			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			uint8_t const pp = msg.read_byte ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_set_pp (party, slot, pp);
*/			break;
		}
		case InMessage::BATTLE_FAINTED: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_fainted (party, slot, nickname);
			break;
		}
		case InMessage::BATTLE_BEGIN_TURN: {
			uint32_t const field_id = msg.read_int ();
			uint16_t const turn_count = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_begin_turn (turn_count);
			break;
		}
		case InMessage::SPECTATOR_BEGIN: {
/*			uint32_t const field_id = msg.read_int ();
			std::string const trainer1 = msg.read_string ();
			std::string const trainer2 = msg.read_string ();
			uint8_t const party_size = msg.read_byte ();
			uint8_t const max_team_length () = msg.read_byte ();
			bool const timing = msg.read_byte ();
			int32_t periods = -1;
			if (timing)
				periods = msg.read_int ();
			for (fill this in later) {
			}
*/			break;
		}
		case InMessage::BATTLE_SET_MOVE: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const pokemon = msg.read_byte ();
			uint8_t const move_slot = msg.read_byte ();
			int16_t const new_move = msg.read_short ();
			uint8_t const pp = msg.read_byte ();
			uint8_t const max_pp = msg.read_byte ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_set_move (pokemon, move_slot, new_move, pp, max_pp);
			break;
		}
		case InMessage::METAGAME_LIST: {
			uint16_t const metagame_count = msg.read_short ();
			std::vector <Metagame> metagames;
			for (uint16_t n = 0; n != metagame_count; ++n) {
				Metagame metagame (msg);
				metagames.push_back (metagame);
			}
			handle_metagame_list (metagames);
			break;
		}
		case InMessage::KICK_BAN_MESSAGE: {
			int32_t const channel = msg.read_int ();
			std::string const mod = msg.read_string ();
			std::string const target = msg.read_string ();
			int32_t const date = msg.read_int ();
			break;
		}
		case InMessage::USER_DETAILS: {
/*			std::string const name = msg.read_string ();
			std::string const ip = msg.read_string ();
			uint8_t const number_of_aliases = msg.read_byte ();
			std::vector <std::string> aliases;
			for (uint8_t n = 0; n != number_of_aliases; ++n)
				aliases.push_back (msg.read_string ());
			uint8_t const number_of_bans = msg.read_byte ();
			std::vector <?> thing;
			for (uint8_t n = 0; n != number_of_bans; ++n)
				msg.read_int ();
				msg.read_?? ();
				msg.read_int ();
*/			break;
		}
		case InMessage::USER_MESSAGE: {
			std::string const name = msg.read_string ();
			std::string const message = msg.read_string ();
			uint8_t const size = msg.read_byte ();
			std::vector <std::pair <uint8_t, int32_t> > estimates;
			for (uint8_t n = 0; n != size; ++n) {
				std::pair <uint8_t, int32_t> estimate = { msg.read_byte (), msg.read_int () };
				estimates.push_back (estimate);
			}
			break;
		}
		case InMessage::BATTLE_STATUS_CHANGE: {
			uint32_t const field_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			uint8_t const type = msg.read_byte ();
			uint8_t const radius = msg.read_byte ();
			std::string const id = msg.read_string ();
			std::string const message = msg.read_string ();
			bool const applied = msg.read_byte ();
			break;
		}
		case InMessage::CLAUSE_LIST: {
			uint16_t const clause_size = msg.read_short ();
			std::vector <std::pair <std::string, std::string> > clauses;
			for (uint16_t n = 0; n != clause_size; ++n) {
				std::pair <std::string, std::string> clause = { msg.read_string(), msg.read_string () };
				clauses.push_back (clause);
			}
			break;
		}
		case InMessage::INVALID_TEAM: {
			std::string const user = msg.read_string ();
			uint8_t const size = msg.read_byte ();
			uint16_t const violation_size = msg.read_short ();
			std::vector <int16_t> violation;
			for (unsigned n = 0; n != violation_size; ++n)
				violation.push_back (msg.read_short());
			handle_invalid_team (violation);
			break;
		}
		case InMessage::ERROR_MESSAGE: {
			uint8_t const type = msg.read_byte ();
			std::string const details = msg.read_string ();
			handle_error_message (type, details);
			break;
		}
		case InMessage::PRIVATE_MESSAGE: {
			std::string const user = msg.read_string ();
			std::string const sender = msg.read_string ();
			std::string const message = msg.read_string ();
			handle_private_message (sender, message);
			break;
		}
		case InMessage::IMPORTANT_MESSAGE: {
			int const channel = msg.read_int ();
			std::string const sender = msg.read_string ();
			std::string const message = msg.read_string ();
			handle_important_message (channel, sender, message);
			break;
		}
		default:
			std::cerr << "Unknown code: " << code << '\n';
			break;
	}
	msg.read_header (socket, this);
}

void BotClient::handle_welcome_message (uint32_t version, std::string const & server, std::string const & message) {
	std::cout << server + '\n';
//	std::cout << message + '\n';
}

void BotClient::handle_password_challenge (InMessage msg) {
	std::string challenge = "";
	for (unsigned n = 0; n != 16; ++n)
		challenge += msg.read_byte();
	int secret_style = msg.read_byte();
	std::string salt = msg.read_string();
	std::string response = get_challenge_response (challenge, secret_style, salt);
	OutMessage out (OutMessage::CHALLENGE_RESPONSE);
	for (unsigned n = 0; n != 16; ++n)
		out.write_byte (response [n]);
	out.send (socket);
}

std::string BotClient::get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt) {
	// Hash of the password is the key to decrypting the number sent.
	std::string digest = getSHA256Hash (get_shared_secret (secret_style, salt));
	
	// Decrypt the challenge in the reverse order it was encrypted.
	rijndael_ctx ctx;
	// First use the second half of the bits to decrypt
	rijndael_set_key (& ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 128);
	unsigned char middle [16];
	rijndael_decrypt (& ctx, reinterpret_cast <unsigned char const *> (challenge.c_str()), middle);
	// Then use the first half of the bits to decrypt that decrypted value.
	rijndael_set_key (& ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 128);
	unsigned char result [16];
	rijndael_decrypt (& ctx, middle, result);
	
	// Add 1 to the decrypted number, which is an int stored in the first 4 bytes (all other bytes are 0).
	uint32_t r = (result [0] << 3 * 8) + (result [1] << 2 * 8) + (result [2] << 1 * 8) + result [3] + 1;
	r = htonl (r);

	// I write back into result instead of a new array so that TM supports a potential future improvement in the security of Pokemon Lab. This will allow the protocol to work even if the server checks all of the digits for correctness, instead of just the first four.
	uint8_t * byte = reinterpret_cast <uint8_t *> (& r);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		result [n] = (*(byte + n));
	// Encrypt that incremented value first with the first half of the bits of my hashed password.
	rijndael_set_key (& ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 128);
	rijndael_encrypt (& ctx, result, middle);
	// Then re-encrypt that encrypted value with the second half of the bits of my hashed password.
	rijndael_set_key (& ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 128);
	rijndael_encrypt (& ctx, middle, result);
	std::string response = "";
	for (unsigned n = 0; n != 16; ++n)
		response += result [n];
	return response;
}

enum Secret_Style {
	NONE = 0,
	MD5 = 1,		// This secret style is never actually used.
	VBULLETIN = 2
};

std::string BotClient::get_shared_secret (int secret_style, std::string const & salt) {
	switch (secret_style) {
		case NONE:
			return password;
		case MD5:
			return getMD5HexHash (password);
		case VBULLETIN:
			return getMD5HexHash (getMD5HexHash (password) + salt);
		default:
			std::cerr << "Unknown secret style of " << secret_style << '\n';
			return "";
	}
}

enum Registry_Response {
	NAME_UNAVAILABLE = 0,
	REGISTER_SUCCESS = 1,
	ILLEGAL_NAME = 2,
	TOO_LONG_NAME = 3,
	NONEXISTENT_NAME = 4,
	INVALID_RESPONSE = 5,
	USER_BANNED = 6,
	SUCCESSFUL_LOGIN = 7,
	USER_ALREADY_ON = 8,
	SERVER_FULL = 9
};

void BotClient::handle_registry_response (uint8_t code, std::string const & details) {
	switch (code) {
		case SUCCESSFUL_LOGIN:
			join_channel ("main");
			break;
		case NAME_UNAVAILABLE:
			std::cerr << "Name unavailable.\n";
			break;
		case REGISTER_SUCCESS:
			break;
		case ILLEGAL_NAME:
			std::cerr << "Illegal name.\n";
			break;
		case TOO_LONG_NAME:
			std::cerr << "Name too long.\n";
			break;
		case NONEXISTENT_NAME:
			std::cerr << "Name does not exist.\n";
			break;
		case INVALID_RESPONSE:
			std::cerr << "Invalid response.\n";
			break;
		case USER_BANNED:
			std::cerr << "You are banned.\n";
			break;
		case USER_ALREADY_ON:
			std::cerr << "You are already logged into this server.\n";
			break;
		case SERVER_FULL:
			std::cerr << "The server is full.\n";
			break;
		default:
			std::cerr << "Authentication failed with unknown code: " << static_cast <int> (code) << ". =(\n";
	}
	std::cerr << details + "\n";
}

void BotClient::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	msg.send (socket);
}

void BotClient::part_channel (std::string const & channel) {
	OutMessage msg (OutMessage::PART_CHANNEL);
	std::map <std::string, uint32_t>::iterator it = channels.find (channel);
	if (it != channels.end()) {
		msg.write_int (it->second);
		channels.erase (it);
		msg.send (socket);
	}
}

void BotClient::handle_channel_info (uint32_t id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users) {
	channels.insert (std::pair <std::string, uint32_t> (channel_name, id));
}

void BotClient::handle_channel_join_part (uint32_t id, std::string const & user, bool joining) {
}

void BotClient::handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags) {
}

void BotClient::handle_channel_list (std::vector <Channel> const & channels) {
}

void BotClient::send_battle_challenge (std::string const & opponent) {
	OutMessage msg (OutMessage::OUTGOING_CHALLENGE);
	uint8_t const generation = 1;
	uint32_t const party_size = 1;
	uint32_t const team_length = 6;
	msg.write_challenge (opponent, generation, party_size, team_length);
	msg.send (socket);
}

void BotClient::handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length) {
	bool accepted;
	if (n > 1 or team_length != 6 or (user != "Technical Machine" and user != "david stone"))
		accepted = false;
	else
		accepted = true;
	bool challenger = false;
	handle_finalize_challenge (user, accepted, challenger);
}

void BotClient::handle_finalize_challenge (std::string const & opponent, bool accepted, bool challenger) {
	OutMessage::Message code;
	if (challenger)
		code = OutMessage::CHALLENGE_TEAM;
	else
		code = OutMessage::RESOLVE_CHALLENGE;
	OutMessage msg (code);
	msg.write_string (opponent);
	// If I am the challenger, I don't write the accepted byte.
	if (!challenger)
		msg.write_byte (accepted);
	std::string verb;
	if (accepted) {
		Battle battle (opponent, depth);
		challenges.insert (std::pair <std::string, Battle> (opponent, battle));
		msg.write_team (battle.ai);
		verb = "Accepted";
	}
	else
		verb = "Rejected";
	msg.send (socket);
	print_with_time_stamp (verb + " challenge vs. " + opponent);
}

void BotClient::handle_battle_begin (uint32_t field_id, std::string const & opponent, uint8_t party) {
 	Battle & battle = challenges.find (opponent)->second;
	battle.party = party;
	battles.insert (std::pair <uint32_t, Battle> (field_id, battle));
	challenges.erase (opponent);
	pause_at_start_of_battle ();
}

void BotClient::handle_victory (uint32_t field_id, int16_t party_id) {
	std::string verb;
	Battle & battle = battles.find (field_id)->second;
	if (battle.party == party_id)
		verb = "Won";
	else
		verb = "Lost";
	print_with_time_stamp (verb + " a battle vs. " + battle.foe.player);
	battles.erase (field_id);
}

void BotClient::handle_metagame_list (std::vector <Metagame> const & metagames) {
}

void BotClient::handle_invalid_team (std::vector <int16_t> const & violation) {
	std::cerr << "Invalid team.\n";
	for (std::vector<int16_t>::const_iterator it = violation.begin(); it != violation.end(); ++it) {
		int pokemon = (-(*it + 1)) % 6;
		std::cerr << "Problem at Pokemon " << pokemon << ": ";
		switch (-(*it + pokemon + 1) / 6) {
			case 0:
				std::cerr << "must have 1-4 unique moves.";
				break;
			case 1:
				std::cerr << "cannot learn all moves.";
				break;
			case 2:
				std::cerr << "illegal move combination.";
				break;
			case 3:
				std::cerr << "invalid quantity of PP ups.";
				break;
			case 4:
				std::cerr << "invalid item.";
				break;
			case 5:
				std::cerr << "cannot learn given ability.";
				break;
			case 6:
				std::cerr << "invalid gender.";
				break;
		}
		std::cerr << '\n';
	}
}

void BotClient::handle_error_message (uint8_t code, std::string const & details) const {
	switch (code) {
		case 0:
			std::cerr << "User does not exist.\n";
			break;
		case 1:
			std::cerr << "User not online.\n";
			break;
		case 2:
			std::cerr << "Bad challenge.\n";
			break;
		case 3:
			std::cerr << "Unauthorized action.\n";
			break;
		default:
			std::cerr << "Unknown error. =(\n";
			break;
	}
	std::cerr << details + "\n";
}

void BotClient::handle_important_message (int32_t channel, std::string const & sender, std::string const & message) {
	print_with_time_stamp ("<Important message> " + sender + ": " + message);
}

void BotClient::send_channel_message (uint32_t channel_id, std::string const & message) {
	OutMessage msg (OutMessage::CHANNEL_MESSAGE);
	msg.write_int (channel_id);
	msg.write_string (message);
	msg.send (socket);
}

void BotClient::send_channel_message (std::string channel, std::string const & message) {
	uint32_t channel_id = channels.find (channel)->second;
	send_channel_message (channel_id, message);
}

void BotClient::send_private_message (std::string const & user, std::string const & message) {
	OutMessage msg (OutMessage::PRIVATE_MESSAGE);
	msg.write_string (user);
	msg.write_string (message);
	msg.send (socket);
}

}		// namespace pl
}		// namespace technicalmachine
