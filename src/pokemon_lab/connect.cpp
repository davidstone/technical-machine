// Connect to Pokemon Lab
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
#include <iostream>
#include <string>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "battle.h"
#include "battle_settings.h"
#include "conversion.h"
#include "inmessage.h"
#include "outmessage.h"

#include "../crypt/get_md5.h"
#include "../crypt/get_sha2.h"
#include "../crypt/rijndael.h"

#include "../network/connect.h"

namespace technicalmachine {
namespace pl {

Client::Client (int depth_):
	network::GenericClient (depth_) {
	request_authentication ();
}

void Client::request_authentication () {
	OutMessage message (OutMessage::REQUEST_CHALLENGE);
	message.write_string (username);
	message.send (*socket);
}

void Client::run () {
	constexpr unsigned timer_length_in_seconds = 45;
	reset_timer (timer_length_in_seconds);

	InMessage msg;
	msg.read_header (*socket, this);

	io.run();
}

void Client::send_keep_alive_message () {
	OutMessage msg (OutMessage::CLIENT_ACTIVITY);
	msg.send (*socket);
}

class Channel {
	public:
		std::string name;
		uint8_t type;
		std::string topic;
		uint32_t population;
		explicit Channel (InMessage & msg):
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
		explicit Metagame (InMessage & msg):
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

void Client::handle_message (InMessage::Message code, InMessage & msg) {
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
			// Apparently unused, maybe it's only for connecting to the registry
			break;
		case InMessage::CHANNEL_INFO: {
			uint32_t const channel_id = msg.read_int();
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
			handle_channel_info (channel_id, info, channel_name, topic, channel_flags, users);
			break;
		}
		case InMessage::CHANNEL_JOIN_PART: {
//			uint32_t const channel_id = msg.read_int();
//			std::string const user = msg.read_string();
//			bool const joining = msg.read_byte();
//			handle_channel_join_part (channel_id, user, joining);
			break;
		}
		case InMessage::CHANNEL_STATUS: {
//			uint32_t const channel_id = msg.read_int();
//			std::string const invoker = msg.read_string();
//			std::string const user = msg.read_string();
//			uint32_t const flags = msg.read_int();
//			handle_channel_status (channel_id, invoker, user, flags);
			break;
		}
		case InMessage::CHANNEL_LIST: {
//			uint32_t const number_of_channels = msg.read_int();
//			std::vector <Channel> channels;
//			for (uint32_t n = 0; n != number_of_channels; ++n) {
//				Channel channel (msg);
//				channels.push_back (channel);
//			}
//			handle_channel_list (channels);
			break;
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
//			int32_t const metagame = msg.read_int ();
//			std::vector <uint8_t> clauses;
//			bool timed;
//			int32_t pool;
//			uint8_t periods;
//			int32_t period_length;
//			if (metagame == -1) {
//				uint8_t const clauses_size = msg.read_byte ();
//				for (uint8_t n = 0; n != clauses.size(); ++n) {
//					uint8_t clause = msg.read_byte ();
//					clauses.push_back (clause);
//				}
//				timed = msg.read_byte ();
//				if (timed) {
//					pool = msg.read_int ();
//					periods = msg.read_byte ();
//					period_length = msg.read_int ();
//				}
//			}
			BattleSettings settings (generation, active_party_size, max_team_length);
			handle_incoming_challenge (user, settings);
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
			handle_challenge_withdrawn (opponent);
		}
		case InMessage::BATTLE_BEGIN: {
			uint32_t const battle_id = msg.read_int ();
			std::string const opponent = msg.read_string ();
			uint8_t const party = msg.read_byte ();
//			int16_t const metagame = msg.read_short ();
//			bool const rated = msg.read_byte ();
//			std::string const unique_battle_id = msg.read_string ();
			handle_battle_begin (battle_id, opponent, party);
			break;
		}
		case InMessage::REQUEST_ACTION: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const slot = msg.read_byte ();
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
					int32_t const num_moves = msg.read_int ();
					for (int32_t n = 0; n != num_moves; ++n)
						moves.push_back (msg.read_byte ());
				}
			}
			bool can_switch_to = false;
			for (uint8_t const legal_switch : switches) {
				if (legal_switch) {
					can_switch_to = true;
					break;
				}
			}
			can_switch = can_switch_to;
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_request_action (*this, battle_id, slot, index, replace, switches, can_switch, forced, moves);
			}
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

			// This appears to be data related to visual stuff... Not needed for the bot.
			break;
		}
		case InMessage::BATTLE_PRINT: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const category = msg.read_byte ();
			int16_t const message_id = msg.read_short ();
			uint8_t const argc = msg.read_byte ();
			std::vector <std::string> arguments;
			for (uint8_t n = 0; n != argc; ++n) {
				std::string const argument = msg.read_string ();
				arguments.push_back (argument);
			}
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_print (category, message_id, arguments);
			}
			break;
		}
		case InMessage::BATTLE_END: {
			uint32_t const battle_id = msg.read_int ();
			// I suspect the int16_t may be a typo in the PL protocol.
			// Every other message sends uint8_t for party_id;
			int16_t const party_id = msg.read_short ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				Result result = get_result (battle, party_id);
				handle_battle_end (battle, battle_id, result);
			}
			break;
		}
		case InMessage::BATTLE_USE_MOVE: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			int16_t const move_id = msg.read_short ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				Move::Moves const move = id_to_move (move_id);
				battle.handle_use_move (party, slot, move);
			}
			break;
		}
		case InMessage::BATTLE_WITHDRAW: {
//			uint32_t const battle_id = msg.read_int ();
//			uint8_t const party = msg.read_byte ();
//			uint8_t const slot = msg.read_byte ();
//			std::string const nickname = msg.read_string ();
//			auto const it = battles.find (battle_id);
//			if (it != battles.end ()) {
//				Battle & battle = static_cast <Battle &> (*it->second);
//				battle.handle_withdraw (party, slot, nickname);
//			}
			break;
		}
		case InMessage::BATTLE_SEND_OUT: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			Species const species = id_to_species (msg.read_short ());
			Gender gender;
			gender.from_simulator_int (msg.read_byte ());
			uint8_t const level = msg.read_byte();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_send_out (party, slot, index, nickname, species, gender, level);
			}
			break;
		}
		case InMessage::BATTLE_HEALTH_CHANGE: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			int16_t const change_in_health = msg.read_short ();
			int16_t const remaining_health = msg.read_short ();
			int16_t const denominator = msg.read_short ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_health_change (party, slot, change_in_health, remaining_health, denominator);
			}
			break;
		}
		case InMessage::BATTLE_SET_PP: {
//			uint32_t const battle_id = msg.read_int ();
//			uint8_t const party = msg.read_byte ();
//			uint8_t const slot = msg.read_byte ();
//			uint8_t const pp = msg.read_byte ();
//			auto const it = battles.find (battle_id);
//			if (it != battles.end ()) {
//				Battle & battle = static_cast <Battle &> (*it->second);
//				battle.handle_set_pp (party, slot, pp);
//			}
			break;
		}
		case InMessage::BATTLE_FAINTED: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			// This isn't needed:
			// std::string const nickname = msg.read_string ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_fainted (party, slot);
			}
			break;
		}
		case InMessage::BATTLE_BEGIN_TURN: {
			uint32_t const battle_id = msg.read_int ();
			uint16_t const turn_count = msg.read_short ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_begin_turn (turn_count);
			}
			break;
		}
		case InMessage::SPECTATOR_BEGIN: {
//			uint32_t const battle_id = msg.read_int ();
//			std::string const trainer1 = msg.read_string ();
//			std::string const trainer2 = msg.read_string ();
//			uint8_t const party_size = msg.read_byte ();
//			uint8_t const max_team_length () = msg.read_byte ();
//			bool const timing = msg.read_byte ();
//			int32_t periods = -1;
//			if (timing)
//				periods = msg.read_int ();
//			for (fill this in later) {
//			}
			break;
		}
		case InMessage::BATTLE_SET_MOVE: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const pokemon = msg.read_byte ();
			uint8_t const move_slot = msg.read_byte ();
			int16_t const new_move = msg.read_short ();
			uint8_t const pp = msg.read_byte ();
			uint8_t const max_pp = msg.read_byte ();
			auto const it = battles.find (battle_id);
			if (it != battles.end ()) {
				Battle & battle = static_cast <Battle &> (*it->second);
				battle.handle_set_move (pokemon, move_slot, new_move, pp, max_pp);
			}
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
			uint32_t const channel_id = msg.read_int ();
			std::string const mod = msg.read_string ();
			std::string const target = msg.read_string ();
			int32_t const date = msg.read_int ();
			break;
		}
		case InMessage::USER_DETAILS: {
//			std::string const name = msg.read_string ();
//			std::string const ip = msg.read_string ();
//			uint8_t const number_of_aliases = msg.read_byte ();
//			std::vector <std::string> aliases;
//			for (uint8_t n = 0; n != number_of_aliases; ++n)
//				aliases.push_back (msg.read_string ());
//			uint8_t const number_of_bans = msg.read_byte ();
//			std::vector <?> thing;
//			for (uint8_t n = 0; n != number_of_bans; ++n)
//				msg.read_int ();
//				msg.read_?? ();
//				msg.read_int ();
			break;
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
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const index = msg.read_byte ();
			uint8_t const type = msg.read_byte ();
			uint8_t const radius = msg.read_byte ();
			std::string const effect_id = msg.read_string ();
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
			uint32_t const channel_id = msg.read_int ();
			std::string const sender = msg.read_string ();
			std::string const message = msg.read_string ();
			handle_server_message (sender, message);
			break;
		}
		default:
			print_with_time_stamp (std::cerr, "Unknown code: " + boost::lexical_cast <std::string> (code));
			break;
	}
	msg.read_header (*socket, this);
}

void Client::handle_welcome_message (uint32_t version, std::string const & server, std::string const & message) const {
	print_with_time_stamp (std::cout, server);
	print_with_time_stamp (std::cout, message);
}

void Client::handle_password_challenge (InMessage msg) {
	std::string challenge = "";
	for (unsigned n = 0; n != 16; ++n)
		challenge += msg.read_byte();
	int secret_style = msg.read_byte();
	std::string salt = msg.read_string();
	std::string response = get_challenge_response (challenge, secret_style, salt);
	OutMessage out (OutMessage::CHALLENGE_RESPONSE);
	for (unsigned n = 0; n != 16; ++n)
		out.write_byte (response [n]);
	out.send (*socket);
}

std::string Client::get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt) {
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

std::string Client::get_shared_secret (int secret_style, std::string const & salt) {
	switch (secret_style) {
		case NONE:
			return password;
		case MD5:
			return getMD5HexHash (password);
		case VBULLETIN:
			return getMD5HexHash (getMD5HexHash (password) + salt);
		default:
			print_with_time_stamp (std::cerr, "Unknown secret style of " + boost::lexical_cast <std::string> (secret_style));
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

void Client::handle_registry_response (uint8_t code, std::string const & details) {
	switch (code) {
		case SUCCESSFUL_LOGIN:
			join_channel ("main");
			break;
		case NAME_UNAVAILABLE:
			print_with_time_stamp (std::cerr, "Name unavailable.");
			break;
		case REGISTER_SUCCESS:
			print_with_time_stamp (std::cout, "Successfully registered name.");
			break;
		case ILLEGAL_NAME:
			print_with_time_stamp (std::cerr, "Illegal name.");
			break;
		case TOO_LONG_NAME:
			print_with_time_stamp (std::cerr, "Name too long.");
			break;
		case NONEXISTENT_NAME:
			print_with_time_stamp (std::cerr, "Name does not exist.");
			break;
		case INVALID_RESPONSE:
			print_with_time_stamp (std::cerr, "Invalid response.");
			break;
		case USER_BANNED:
			print_with_time_stamp (std::cerr, "You are banned.");
			break;
		case USER_ALREADY_ON:
			print_with_time_stamp (std::cerr, "You are already logged into this server.");
			break;
		case SERVER_FULL:
			print_with_time_stamp (std::cerr, "The server is full.");
			break;
		default:
			print_with_time_stamp (std::cerr, "Authentication failed with unknown code: " + boost::lexical_cast <std::string> (code) + ". =(");
	}
	if (!details.empty ())
		print_with_time_stamp (std::cerr, details);
}

void Client::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	msg.send (*socket);
}

void Client::part_channel (std::string const & channel) {
	auto const it = channels.find (channel);
	if (it != channels.end()) {
		OutMessage msg (OutMessage::PART_CHANNEL);
		msg.write_int (it->second);
		channels.erase (it);
		msg.send (*socket);
	}
}

void Client::handle_channel_info (uint32_t channel_id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users) {
	channels.insert (std::pair <std::string, uint32_t> (channel_name, channel_id));
}

void Client::handle_channel_join_part (uint32_t channel_id, std::string const & user, bool joining) {
}

void Client::handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags) {
}

void Client::handle_channel_list (std::vector <Channel> const & channels) {
}

void Client::send_battle_challenge (std::string const & opponent) {
	OutMessage msg (OutMessage::OUTGOING_CHALLENGE);
	uint8_t const generation = 1;
	uint32_t const party_size = 1;
	uint32_t const team_length = 6;
	msg.write_challenge (opponent, generation, party_size, team_length);
	msg.send (*socket);
}

void Client::handle_finalize_challenge (std::string const & opponent, bool accepted, bool challenger) {
	OutMessage::Message const code = challenger ? OutMessage::CHALLENGE_TEAM : OutMessage::RESOLVE_CHALLENGE;
	OutMessage msg (code);
	msg.write_string (opponent);
	// If I am the challenger, I don't write the accepted byte.
	if (!challenger)
		msg.write_byte (accepted);
	std::string verb;
	if (accepted) {
		std::shared_ptr <Battle> battle (new Battle (opponent, depth));
		add_pending_challenge (battle);
		msg.write_team (battle->ai);
		verb = "Accepted";
	}
	else
		verb = "Rejected";
	msg.send (*socket);
	print_with_time_stamp (std::cout, verb + " challenge vs. " + opponent);
}

void Client::handle_metagame_list (std::vector <Metagame> const & metagames) {
}

void Client::handle_invalid_team (std::vector <int16_t> const & violations) {
	print_with_time_stamp (std::cerr, "Invalid team.");
	for (int16_t const violation : violations) {
		int pokemon = (-(violation + 1)) % 6;
		std::string output = "Problem at Pokemon " + boost::lexical_cast <std::string> (pokemon) + ": ";
		switch (-(violation + pokemon + 1) / 6) {
			case 0:
				output += "must have 1-4 unique moves.";
				break;
			case 1:
				output += "cannot learn all moves.";
				break;
			case 2:
				output += "illegal move combination.";
				break;
			case 3:
				output += "invalid quantity of PP ups.";
				break;
			case 4:
				output += "invalid item.";
				break;
			case 5:
				output += "cannot learn given ability.";
				break;
			case 6:
				output += "invalid gender.";
				break;
		}
		print_with_time_stamp (std::cerr, output);
	}
}

void Client::handle_error_message (uint8_t code, std::string const & details) const {
	switch (code) {
		case 0:
			print_with_time_stamp (std::cerr, "User does not exist.");
			break;
		case 1:
			print_with_time_stamp (std::cerr, "User not online.");
			break;
		case 2:
			print_with_time_stamp (std::cerr, "Bad challenge.");
			break;
		case 3:
			print_with_time_stamp (std::cerr, "Unauthorized action.");
			break;
		default:
			print_with_time_stamp (std::cerr, "Unknown error. =(");
			break;
	}
	if (!details.empty ())
		print_with_time_stamp (std::cerr, details);
}

void Client::send_channel_message (uint32_t channel_id, std::string const & message) {
	OutMessage msg (OutMessage::CHANNEL_MESSAGE);
	msg.write_int (channel_id);
	msg.write_string (message);
	msg.send (*socket);
}

void Client::send_private_message (std::string const & user, std::string const & message) {
	OutMessage msg (OutMessage::PRIVATE_MESSAGE);
	msg.write_string (user);
	msg.write_string (message);
	msg.send (*socket);
}

network::GenericClient::Result Client::get_result (Battle const & battle, int16_t party_id) const {
	Result result;
	if (party_id != -1)
		result = (battle.party == party_id) ? WON : LOST;
	else
		result = TIED;
	return result;
}

} // namespace pl
} // namespace technicalmachine
