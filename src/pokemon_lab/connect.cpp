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

#include "connect.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <boost/bind.hpp>

#include "battle.hpp"
#include "battle_settings.hpp"
#include "conversion.hpp"
#include "inmessage.hpp"
#include "outmessage.hpp"

#include "../battle_result.hpp"

#include "../cryptography/byte_order.hpp"
#include "../cryptography/md5.hpp"
#include "../cryptography/sha2.hpp"
#include "../cryptography/rijndael.h"

#include "../network/connect.hpp"

namespace technicalmachine {
namespace pl {

Client::Client (unsigned set_depth):
	network::GenericClient (set_depth),
	timer (io)
	{
	request_authentication ();
}

void Client::request_authentication () {
	OutMessage message (OutMessage::REQUEST_CHALLENGE);
	message.write_string (username());
	send_message(message);
}

void Client::reset_timer (unsigned timer_length) {
	send_keep_alive_message ();
	timer.expires_from_now (boost::posix_time::seconds (timer_length));
	timer.async_wait (boost::bind (& Client::reset_timer, this, timer_length));
}

void Client::run () {
	constexpr unsigned timer_length_in_seconds = 45;
	reset_timer (timer_length_in_seconds);

	InMessage msg;
	read_header(msg);

	io.run();
}

void Client::send_keep_alive_message () {
	OutMessage msg (OutMessage::CLIENT_ACTIVITY);
	send_message(msg);
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
				bans.emplace_back(species_id);
			}
		}
		void load_clauses (InMessage & msg) {
			uint16_t clause_count = msg.read_short ();
			for (uint16_t b = 0; b != clause_count; ++b) {
				std::string clause_name = msg.read_string ();
				clauses.emplace_back(clause_name);
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
			pool_length (0xFFFF),
			periods (0xFF),
			period_length (0xFFFF) {
			load_bans (msg);
			load_clauses (msg);
			load_battle_timer (msg);
		}
};

void Client::handle_message (InMessage::Message code, InMessage & msg) {
	switch (code) {
		case InMessage::WELCOME_MESSAGE: {
			uint32_t const version = msg.read_int();
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
				std::string const name_of_user = msg.read_string();
				uint32_t const flags = msg.read_int();
				users.emplace_back(name_of_user, flags);
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
//				channels.emplace_back(msg);
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
//					clauses.emplace_back(msg.read_byte());
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
			break;
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
			bool can_switch = false;
			for (uint32_t n = 0; n != number_of_pokemon; ++n) {
				bool const legal_to_switch_to_this_pokemon = msg.read_byte ();
				// Rather than the more straightforward
				//	can_switch = msg.read_byte ();
				//	if (can_switch)
				//		break;
				// I use |= so that I read the correct number of bytes but
				// still set can_switch to true if it's legal to switch to any
				// Pokemon.
				can_switch |= legal_to_switch_to_this_pokemon;
			}

			bool forced = false;
			std::vector <uint8_t> moves;
			if (!replace) {
				// Not sure what this is used for.
				bool const read_can_switch = msg.read_byte ();
				if (read_can_switch != can_switch)
					print_with_time_stamp (std::cerr, "Conflicting switch legalities.");
				forced = msg.read_byte ();
				if (!forced) {
					uint32_t const num_moves = msg.read_int ();
					for (uint32_t n = 0; n != num_moves; ++n)
						moves.emplace_back(msg.read_byte ());
				}
			}
			auto & battle = battles.find(battle_id);
			OutMessage action (OutMessage::BATTLE_ACTION);
			battle.handle_request_action (*this, action, battle_id, can_switch, moves, forced);
			send_message(action);
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
			uint16_t const message_id = msg.read_short ();
			uint8_t const argc = msg.read_byte ();
			std::vector <std::string> arguments;
			for (uint8_t n = 0; n != argc; ++n) {
				arguments.emplace_back(msg.read_string());
			}
			auto & battle = static_cast<Battle &>(battles.find (battle_id));
			battle.handle_print (category, message_id, arguments);
			break;
		}
		case InMessage::BATTLE_END: {
			uint32_t const battle_id = msg.read_int ();
			// I'm interpreting party_id as uint16_t instead of int16_t.
			uint16_t const winner = msg.read_short ();
			handle_battle_end(battle_id, get_result(battle_id, winner));
			break;
		}
		case InMessage::BATTLE_USE_MOVE: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			std::string const nickname = msg.read_string ();
			uint16_t const move_id = msg.read_short ();
			auto & battle = battles.find(battle_id);
			battle.handle_use_move (party, slot, id_to_move(move_id));
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
			Gender gender (id_to_gender (msg.read_byte ()));
			uint8_t const level = msg.read_byte();
			auto & battle = battles.find (battle_id);
			battle.handle_send_out (party, slot, index, nickname, species, gender, level);
			break;
		}
		case InMessage::BATTLE_HEALTH_CHANGE: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			int16_t const change_in_hp = static_cast<int16_t> (msg.read_short ());
			assert (change_in_hp > 0);
			uint16_t const remaining_hp = msg.read_short ();
			int16_t const denominator = static_cast<int16_t> (msg.read_short ());
			assert(denominator > 0);
			auto & battle = battles.find(battle_id);
			battle.handle_hp_change (party, slot, static_cast<uint16_t>(change_in_hp), remaining_hp, static_cast<uint16_t>(denominator));
			break;
		}
		case InMessage::BATTLE_SET_PP: {
//			uint32_t const battle_id = msg.read_int ();
//			uint8_t const party = msg.read_byte ();
//			uint8_t const slot = msg.read_byte ();
//			uint8_t const pp = msg.read_byte ();
//			auto & battle = battles.find(battle_id);
//			battle.handle_set_pp (party, slot, pp);
			break;
		}
		case InMessage::BATTLE_FAINTED: {
			uint32_t const battle_id = msg.read_int ();
			uint8_t const party = msg.read_byte ();
			uint8_t const slot = msg.read_byte ();
			// This isn't needed:
			// std::string const nickname = msg.read_string ();
			auto & battle = battles.find(battle_id);
			battle.handle_fainted (party, slot);
			break;
		}
		case InMessage::BATTLE_BEGIN_TURN: {
			uint32_t const battle_id = msg.read_int ();
			uint16_t const turn_count = msg.read_short ();
			auto & battle = battles.find(battle_id);
			battle.handle_begin_turn (turn_count);
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
			int16_t const new_move = static_cast<int16_t> (msg.read_short ());
			uint8_t const pp = msg.read_byte ();
			uint8_t const max_pp = msg.read_byte ();
			auto & battle = static_cast<Battle &>(battles.find (battle_id));
			battle.handle_set_move (pokemon, move_slot, new_move, pp, max_pp);
			break;
		}
		case InMessage::METAGAME_LIST: {
			uint16_t const metagame_count = msg.read_short ();
			std::vector <Metagame> metagames;
			for (uint16_t n = 0; n != metagame_count; ++n) {
				metagames.emplace_back(msg);
			}
			handle_metagame_list (metagames);
			break;
		}
		case InMessage::KICK_BAN_MESSAGE: {
			uint32_t const channel_id = msg.read_int ();
			std::string const mod = msg.read_string ();
			std::string const target = msg.read_string ();
			int32_t const date = static_cast<int32_t> (msg.read_int ());
			break;
		}
		case InMessage::USER_DETAILS: {
//			std::string const name = msg.read_string ();
//			std::string const ip = msg.read_string ();
//			uint8_t const number_of_aliases = msg.read_byte ();
//			std::vector <std::string> aliases;
//			for (uint8_t n = 0; n != number_of_aliases; ++n)
//				aliases.emplace_back(msg.read_string ());
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
				estimates.emplace_back(msg.read_byte(), msg.read_int());
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
				clauses.emplace_back(msg.read_string(), msg.read_string());
			}
			break;
		}
		case InMessage::INVALID_TEAM: {
			std::string const user = msg.read_string ();
			uint8_t const size = msg.read_byte ();
			uint16_t const violation_size = msg.read_short ();
			std::vector <int16_t> violation;
			for (unsigned n = 0; n != violation_size; ++n)
				violation.emplace_back(static_cast<int16_t> (msg.read_short()));
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
			print_with_time_stamp (std::cerr, "Unknown code: " + std::to_string (static_cast<unsigned> (code)));
			break;
	}
	read_header(msg);
}

void Client::handle_welcome_message (uint32_t version, std::string const & server, std::string const & message) const {
	print_with_time_stamp (std::cout, server);
	print_with_time_stamp (std::cout, message);
}

void Client::handle_password_challenge (InMessage & msg) {
	std::string challenge = "";
	for (unsigned n = 0; n != 16; ++n)
		challenge += static_cast<char> (msg.read_byte());
	SecretStyle secret_style = static_cast<SecretStyle> (msg.read_byte());
	std::string salt = msg.read_string();
	std::string response = get_challenge_response (challenge, secret_style, salt);
	OutMessage out (OutMessage::CHALLENGE_RESPONSE);
	for (unsigned n = 0; n != 16; ++n)
		out.write_byte (static_cast<uint8_t> (response [n]));
	send_message(out);
}

std::string Client::get_challenge_response (std::string const & challenge, SecretStyle secret_style, std::string const & salt) {
	// Hash of the password is the key to decrypting the number sent.
	std::string digest = cryptography::sha256 (get_shared_secret (secret_style, salt));
	
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
	uint32_t r = 1;
	for (unsigned n = 0; n != 4; ++n)
		r += static_cast<uint32_t> (result [n]) << ((3 - n) * 8);
	r = boost::endian::h_to_n(r);

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
	return std::string (reinterpret_cast<char *> (result), 16);
}

std::string Client::get_shared_secret (SecretStyle secret_style, std::string const & salt) {
	switch (secret_style) {
		case NONE:
			return password();
		case MD5:
			return cryptography::md5(password());
		case VBULLETIN:
			return cryptography::md5(cryptography::md5(password()) + salt);
		default:
			print_with_time_stamp (std::cerr, "Unknown secret style of " + std::to_string (static_cast<unsigned> (secret_style)));
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
			print_with_time_stamp (std::cerr, "Authentication failed with unknown code: " + std::to_string (static_cast<unsigned> (code)) + ". =(");
	}
	if (!details.empty ())
		print_with_time_stamp (std::cerr, details);
}

void Client::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	send_message(msg);
}

void Client::part_channel (std::string const & channel) {
	auto const it = channels.find (channel);
	if (it != channels.end()) {
		OutMessage msg (OutMessage::PART_CHANNEL);
		msg.write_int (it->second);
		channels.erase (it);
		send_message(msg);
	}
}

void Client::handle_channel_info (uint32_t channel_id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users) {
	channels.insert (std::pair <std::string, uint32_t> (channel_name, channel_id));
}

void Client::handle_channel_join_part (uint32_t channel_id, std::string const & user, bool joining) {
}

void Client::handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags) {
}

void Client::handle_channel_list (std::vector <Channel> const & channel_list) {
}

void Client::send_battle_challenge (std::string const & opponent) {
	OutMessage msg (OutMessage::OUTGOING_CHALLENGE);
	uint8_t const generation = 1;
	uint32_t const party_size = 1;
	uint32_t const team_length = 6;
	msg.write_challenge (opponent, generation, party_size, team_length);
	send_message(msg);
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
		auto const & battle = add_pending_challenge<Battle>(opponent);
		battle.write_team(msg);
		verb = "Accepted";
	}
	else
		verb = "Rejected";
	send_message(msg);
	print_with_time_stamp (std::cout, verb + " challenge vs. " + opponent);
}

void Client::handle_metagame_list (std::vector <Metagame> const & metagames) {
}

void Client::handle_invalid_team (std::vector <int16_t> const & violations) {
	print_with_time_stamp (std::cerr, "Invalid team.");
	for (int16_t const violation : violations) {
		int const pokemon = (-(violation + 1)) % 6;
		std::string output = "Problem at Pokemon " + std::to_string (pokemon) + ": ";
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
			default:
				assert (false);
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
	send_message(msg);
}

void Client::send_private_message (std::string const & user, std::string const & message) {
	OutMessage msg (OutMessage::PRIVATE_MESSAGE);
	msg.write_string (user);
	msg.write_string (message);
	send_message(msg);
}

Result Client::get_result (uint32_t const battle_id, uint16_t const winner) {
	if (winner != 0xFFFF)
		return battles.find(battle_id).is_me (winner) ? Result::won : Result::lost;
	else
		return Result::tied;
}

} // namespace pl
} // namespace technicalmachine
