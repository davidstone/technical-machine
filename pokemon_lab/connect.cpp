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

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../crypt/get_md5.h"
#include "../crypt/get_sha2.h"
#include "../crypt/rijndael.h"

#include "../analyze_logs.h"
#include "../evaluate.h"
#include "../expectiminimax.h"
#include "../load_stats.h"
#include "../map.h"
#include "../pokemon.h"
#include "../species.h"
#include "../team.h"
#include "../teampredictor.h"
#include "../weather.h"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/asio/error.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "connect.h"
#include "inmessage.h"
#include "outmessage.h"
#undef SING

namespace technicalmachine {
namespace pl {

Battle::Battle (Map const & map, std::string const & opponent, uint8_t party_):
	ai (true, map, 6),
	foe (false, map, ai.size),
	log (ai, foe),
	party (party_) {
	foe.player = opponent;
	ai.replacing = true;
	foe.replacing = true;
}

BotClient::BotClient (int depth_):
	detailed ({{ 0 }}),
	depth (depth_),
	timer (io, boost::posix_time::seconds (45)),
	socket (io) {
	srand (static_cast <unsigned> (time (0)));
	load_responses ();
	detailed_stats (map, detailed);
	std::string host;
	std::string port;
	account_info (host, port);
	connect (host, port);
	authenticate ();
}

void BotClient::account_info (std::string & host, std::string & port) {
	std::ifstream file ("account.txt");
	std::string line;
	std::string delimiter = ": ";
	for (getline (file, line); !file.eof(); getline (file, line)) {
		size_t position = line.find (delimiter);
		std::string data = line.substr (0, position);
		if (data == "host")
			host = line.substr (position + delimiter.length());
		else if (data == "port")
			port = line.substr (position + delimiter.length());
		else if (data == "username")
			username = line.substr (position + delimiter.length());
		else if (data == "password")
			password = line.substr (position + delimiter.length());
	}
	file.close();
}

void BotClient::connect (std::string const & host, std::string const & port) {
	boost::asio::ip::tcp::resolver resolver (io);
	boost::asio::ip::tcp::resolver::query query (host, port);
	boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve (query);
	boost::asio::ip::tcp::resolver::iterator end;
	boost::system::error_code error = boost::asio::error::host_not_found;
	while (error and endpoint_iterator != end) {
		socket.close();
		socket.connect (*endpoint_iterator++, error);
	}
}

void BotClient::authenticate () {
	OutMessage message (OutMessage::REQUEST_CHALLENGE);
	message.write_string (username);
	message.send (socket);
}

std::string BotClient::get_shared_secret (int secret_style, std::string const & salt) {
	switch (secret_style) {
		case 0:
			return password;
		case 1:
			return getMD5HexHash (password);
		case 2:
			return getMD5HexHash (getMD5HexHash (password) + salt);
		default:
			std::cerr << "Unknown secret style of " << secret_style << '\n';
			return "";
	}
}

std::string BotClient::get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt) {
	// Hash of the password is the key to decrypting the number sent.
	std::string digest = getSHA256Hash (get_shared_secret (secret_style, salt));
	
	// Decrypt the challenge in the reverse order it was encrypted.
	rijndael_ctx ctx;
	// First use the second half of the bits to decrypt
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 128);
	unsigned char middle [16];
	rijndael_decrypt (&ctx, reinterpret_cast <unsigned char const *> (challenge.c_str()), middle);
	// Then use the first half of the bits to decrypt that decrypted value.
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 128);
	unsigned char result [16];
	rijndael_decrypt (&ctx, middle, result);
	
	// Add 1 to the decrypted number, which is an int stored in the first 4 bytes (all other bytes are 0).
	uint32_t r = (result [0] << 3 * 8) + (result [1] << 2 * 8) + (result [2] << 1 * 8) + result [3] + 1;
	r = htonl (r);

	// I write back into result instead of a new array so that TM supports a potential future improvement in the security of Pokemon Lab. This will allow the protocol to work even if the server checks all of the digits for correctness, instead of just the first four.
	uint8_t * byte = reinterpret_cast <uint8_t *> (&r);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		result [n] = (*(byte + n));
	// Encrypt that incremented value first with the first half of the bits of my hashed password.
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 128);
	rijndael_encrypt (&ctx, result, middle);
	// Then re-encrypt that encrypted value with the second half of the bits of my hashed password.
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 128);
	rijndael_encrypt (&ctx, middle, result);
	std::string response = "";
	for (unsigned n = 0; n != 16; ++n)
		response += result [n];
	return response;
}

void BotClient::run () {
	timer.async_wait (boost::bind (& BotClient::reset_timer, this, _1));

	InMessage msg;
	msg.read_header (socket, this);

	io.run();
}

void BotClient::reset_timer (boost::system::error_code const & error) {
	if (!error) {
		OutMessage msg (OutMessage::CLIENT_ACTIVITY);
		msg.send (socket);
		timer.expires_from_now (boost::posix_time::seconds (45));
		timer.async_wait (boost::bind (& BotClient::reset_timer, this, _1));
	}
	else {
		std::cerr << "An error has occured handling the keep alive timer! =o\n";
		std::cerr << "Error code: " << error << '\n';
	}
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
		void load_bans (InMessage &msg) {
			uint16_t ban_list_count = msg.read_short ();
			for (uint16_t n = 0; n != ban_list_count; ++n) {
				uint16_t species_id = msg.read_short ();
				bans.push_back (species_id);
			}
		}
		void load_clauses (InMessage &msg) {
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
			uint32_t version = msg.read_int();
			std::string server_name = msg.read_string();
			std::string message = msg.read_string();
			handle_welcome_message (version, server_name, message);
			break;
		}
		case InMessage::PASSWORD_CHALLENGE:
			handle_challenge (msg);
			break;
		case InMessage::REGISTRY_RESPONSE: {
			uint8_t type = msg.read_byte();
			std::string details = msg.read_string();
			handle_registry_response (type, details);
			break;
		}
		case InMessage::CHANNEL_INFO: {
			uint32_t id = msg.read_int();
			uint8_t info = msg.read_byte();
			std::string channel_name = msg.read_string();
			std::string topic = msg.read_string();
			uint32_t channel_flags = msg.read_int();
			uint32_t number_of_users = msg.read_int();
			std::vector <std::pair <std::string, uint32_t> > users;
			for (uint32_t n = 0; n != number_of_users; ++n) {
				std::string username = msg.read_string();
				uint32_t flags = msg.read_int();
				std::pair <std::string, uint32_t> user (username, flags);
				users.push_back (user);
			}
			handle_channel_info (id, info, channel_name, topic, channel_flags, users);
			break;
		}
		case InMessage::CHANNEL_JOIN_PART: {
			uint32_t id = msg.read_int();
			std::string user = msg.read_string();
			bool joining = msg.read_byte();
			handle_channel_join_part (id, user, joining);
			break;
		}
		case InMessage::CHANNEL_STATUS: {
			uint32_t channel_id = msg.read_int();
			std::string invoker = msg.read_string();
			std::string user = msg.read_string();
			uint32_t flags = msg.read_int();
			handle_channel_status (channel_id, invoker, user, flags);
			break;
		}
		case InMessage::CHANNEL_LIST: {
			uint32_t number_of_channels = msg.read_int();
			std::vector <Channel> channels;
			for (uint32_t n = 0; n != number_of_channels; ++n) {
				Channel channel (msg);
				channels.push_back (channel);
			}
			handle_channel_list (channels);
			break;
		}
		case InMessage::CHANNEL_MESSAGE: {
			uint32_t channel_id = msg.read_int();
			std::string user = msg.read_string ();
			std::string message = msg.read_string ();
			handle_channel_message (channel_id, user, message);
			break;
		}
		case InMessage::INCOMING_CHALLENGE: {
			std::string user = msg.read_string ();
			uint8_t generation = msg.read_byte ();
			uint32_t active_party_size = msg.read_int ();
			uint32_t max_team_length = msg.read_int ();
			handle_incoming_challenge (user, generation, active_party_size, max_team_length);
			break;
		}
		case InMessage::FINALIZE_CHALLENGE: {
			std::string user = msg.read_string ();
			bool accepted = msg.read_byte ();
			handle_finalize_challenge (user, accepted);
			break;
		}
		case InMessage::BATTLE_BEGIN: {
			uint32_t field_id = msg.read_int ();
			std::string opponent = msg.read_string ();
			uint8_t party = msg.read_byte ();
			handle_battle_begin (field_id, opponent, party);
			break;
		}
		case InMessage::REQUEST_ACTION: {
			uint32_t field_id = msg.read_int ();
			uint8_t slot = msg.read_byte ();
			uint8_t index = msg.read_byte ();
			bool replace = msg.read_byte ();
			uint8_t request_sequences = msg.read_byte ();
			uint8_t sequential_requests = msg.read_byte ();
			uint32_t number_of_pokemon = msg.read_int ();
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
			handle_request_action (field_id, slot, index, replace, switches, can_switch, forced, moves);
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
		   
			// the bot probably doesn't need to care about this
			std::cout << "handle_battle_pokemon\n";
			msg.skip ();
			break;
		}
		case InMessage::BATTLE_PRINT: {
			uint32_t field_id = msg.read_int ();
			uint8_t category = msg.read_byte ();
			uint16_t message_id = msg.read_short ();
			uint8_t argc = msg.read_byte ();
			std::vector <std::string> arguments;
			for (uint8_t n = 0; n != argc; ++n) {
				std::string argument = msg.read_string ();
				arguments.push_back (argument);
			}
			handle_battle_print (field_id, category, message_id, arguments);
			break;
		}
		case InMessage::BATTLE_VICTORY: {
			uint32_t field_id = msg.read_int ();
			uint16_t party_id = msg.read_short ();
			handle_battle_victory (field_id, party_id);
			break;
		}
		case InMessage::BATTLE_USE_MOVE: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			uint16_t move_id = msg.read_short ();
			handle_battle_use_move (field_id, party, slot, nickname, move_id);
			break;
		}
		case InMessage::BATTLE_WITHDRAW: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			handle_battle_withdraw (field_id, party, slot, nickname);
			break;
		}
		case InMessage::BATTLE_SEND_OUT: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			uint8_t index = msg.read_byte ();
			std::string nickname = msg.read_string ();
			uint16_t species_id = msg.read_short ();
			uint8_t gender = msg.read_byte ();
			uint8_t level = msg.read_byte();
			handle_battle_send_out (field_id, party, slot, index, nickname, species_id, gender, level);
			break;
		}
		case InMessage::BATTLE_HEALTH_CHANGE: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			uint16_t change_in_health = msg.read_short ();
			uint16_t remaining_health = msg.read_short ();
			uint16_t denominator = msg.read_short ();
			handle_battle_health_change (field_id, party, slot, change_in_health, remaining_health, denominator);
			break;
		}
		case InMessage::BATTLE_SET_PP: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			uint8_t pp = msg.read_byte ();
			handle_battle_set_pp (field_id, party, slot, pp);
			break;
		}
		case InMessage::BATTLE_FAINTED: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			handle_battle_fainted (field_id, party, slot, nickname);
			break;
		}
		case InMessage::BATTLE_BEGIN_TURN: {
			uint32_t field_id = msg.read_int ();
			uint16_t turn_count = msg.read_short ();
			handle_battle_begin_turn (field_id, turn_count);
			break;
		}
		case InMessage::SPECTATOR_BEGIN: {
			msg.skip();
			break;
		}
		case InMessage::BATTLE_SET_MOVE: {
			uint32_t field_id = msg.read_int ();
			uint8_t pokemon = msg.read_byte ();
			uint8_t move_slot = msg.read_byte ();
			uint16_t new_move = msg.read_short ();
			uint8_t pp = msg.read_byte ();
			uint8_t max_pp = msg.read_byte ();
			handle_battle_set_move (field_id, pokemon, move_slot, new_move, pp, max_pp);
			break;
		}
		case InMessage::METAGAME_LIST: {
			uint16_t metagame_count = msg.read_short ();
			std::vector <Metagame> metagames;
			for (uint16_t n = 0; n != metagame_count; ++n) {
				Metagame metagame (msg);
				metagames.push_back (metagame);
			}
			handle_metagame_list (metagames);
			break;
		}
		case InMessage::INVALID_TEAM: {
			std::string user = msg.read_string ();
			uint8_t size = msg.read_byte ();
			uint16_t violation_size = msg.read_short ();
			std::vector <int16_t> violation;
			for (unsigned n = 0; n != violation_size; ++n)
				violation.push_back (msg.read_short());
			handle_invalid_team (violation);
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
	std::cout << message + '\n';
}

void BotClient::handle_challenge (InMessage msg) {
	std::string challenge = "";
	for (unsigned n = 0; n != 16; ++n)
		challenge += msg.read_byte();
	int secret_style = msg.read_byte();
	std::string salt = msg.read_string();
	std::string response = get_challenge_response (challenge, secret_style, salt);
	OutMessage out (1);
	for (unsigned n = 0; n != 16; ++n)
		out.write_byte (response [n]);
	out.send (socket);
}

void BotClient::handle_registry_response (uint8_t type, std::string const & details) {
	if (type == 7)
		join_channel ("main");
	else {
		std::cerr << "Authentication failed with code: " << static_cast <int> (type) << ". =(\n";
		if (details.length() > 0)
			std::cerr << details + "\n";
	}
}

void BotClient::handle_channel_info (uint32_t id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users) {
	std::cout << "handle_channel_info\n";
}

void BotClient::handle_channel_join_part (uint32_t id, std::string const & user, bool joining) {
}

void BotClient::handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags) {
	std::cout << "handle_channel_status\n";
}

void BotClient::handle_channel_list (std::vector <Channel> const & channels) {
	std::cout << "handle_channel_list\n";
}

void BotClient::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) {
	// Vanity mode!
	std::string msg = message;
	boost::to_lower (msg);
	if (msg.find ("obi") != std::string::npos or msg.find ("david stone") != std::string::npos or msg.find ("technical machine") != std::string::npos or msg.find ("tm") != std::string::npos)
		std::cout << message + "\n"; 
}

void BotClient::handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length) {
	if (n > 1 or team_length != 6 or user != "Technical Machine")
		reject_challenge (user);
	else
		accept_challenge (user);
}

void BotClient::handle_finalize_challenge (std::string const & user, bool accepted) {
	std::cout << "handle_finalize_challenge\n";
}

void BotClient::handle_battle_begin (uint32_t field_id, std::string const & opponent, uint8_t party) {
	std::cout << "handle_battle_begin\n";
	Battle battle (map, opponent, party);
	battles [field_id] = battle;
}

void BotClient::handle_request_action (uint32_t field_id, uint8_t slot, uint8_t index, bool replace, std::vector <uint8_t> const & switches, bool can_switch, bool forced, std::vector <uint8_t> const & moves) {
	std::cout << "handle_request_action\n";
	Battle & battle = battles.find (field_id)->second;
	do_turn (*battle.log.first, *battle.log.last, battle.weather);
	if (rand () % 20 == 0) {
		OutMessage comment (OutMessage::CHANNEL_MESSAGE);
		comment.write_int (field_id);
		comment.write_string (response [rand() % response.size()]);
		comment.send (socket);
	}
	OutMessage msg (OutMessage::BATTLE_ACTION);
	if (forced)
		msg.write_move (field_id, 1);
	else {
		Team predicted = battle.foe;
		std::cout << "======================\nPredicting...\n";
		predict_team (detailed, predicted, battle.ai.size);
		std::string out;
		predicted.output (out);
		std::cout << out;

		int64_t score;
		moves_list move = expectiminimax (battle.ai, predicted, battle.weather, depth, sv, score);
		if (Move::is_switch (move))
			msg.write_switch (field_id, move);
		else {
			uint8_t move_index = 0;
			while (battle.ai.pokemon->move.set [move_index].name != move)
				++move_index;
			msg.write_move (field_id, move_index, 1 - battle.party);
		}
	}
	msg.send (socket);
	if (!battle.ai.replacing)
		battle.log.initialize_turn (battle.ai, battle.foe);
}

void BotClient::handle_battle_print (uint32_t field_id, uint8_t category, uint16_t message_id, std::vector <std::string> const & arguments) {
	std::cout << "handle_battle_print\n";
	std::cout << "category: " << static_cast <int> (category) << '\n';
	std::cout << "message id: " << message_id << '\n';
	std::cout << "arguments: " << '\n';
	for (std::vector <std::string>::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
		std::cout << "\t" + *it + "\n";
}

void BotClient::handle_battle_victory (uint32_t field_id, uint16_t party_id) {
	std::cout << "handle_battle_victory\n";
}

void BotClient::handle_battle_use_move (uint32_t field_id, uint8_t party, uint8_t slot, std::string const & nickname, uint16_t move_id) {
	std::cout << "handle_battle_use_move\n";
	Battle & battle = battles.find (field_id)->second;
	Team * team;
	Team * other;
	if (battle.party == party) {
		team = &battle.ai;
		other = &battle.foe;
	}
	else {
		team = &battle.foe;
		other = &battle.ai;
	}
	battle.log.active = team;
	battle.log.inactive = other;
	if (battle.log.first == NULL) {
		battle.log.first = team;
		battle.log.last = other;
	}
	int move = move_id;
	if (move >= SWITCH0)
		move += 6;
	battle.log.log_move (static_cast <moves_list> (move));
}

void BotClient::handle_battle_withdraw (uint32_t field_id, uint8_t party, uint8_t slot, std::string const & nickname) {
	std::cout << "handle_battle_withdraw\n";
}

void BotClient::handle_battle_send_out (uint32_t field_id, uint8_t party, uint8_t slot, uint8_t index, std::string const & nickname, uint16_t species_id, uint8_t gender, uint8_t level) {
	std::cout << "handle_battle_send_out\n";
	Battle & battle = battles.find (field_id)->second;
	Team * team;
	Team * other;
	if (battle.party == party) {
		team = &battle.ai;
		other = &battle.foe;
	}
	else {
		team = &battle.foe;
		other = &battle.ai;
	}
	species name = InMessage::pl_to_tm_species (species_id);
	battle.log.pokemon_sent_out (map, name, nickname, level, static_cast <genders> (gender), *team, *other);
}

void BotClient::handle_battle_health_change (uint32_t field_id, uint8_t party, uint8_t slot, uint16_t change_in_health, uint16_t remaining_health, uint16_t denominator) {
	std::cout << "handle_battle_health_change\n";
	Battle & battle = battles.find (field_id)->second;
	if (battle.log.move_damage) {
		battle.log.active->damage = battle.log.active->at_replacement().hp.max * change_in_health / denominator;
		battle.log.move_damage = false;
	}
}

void BotClient::handle_battle_set_pp (uint32_t field_id, uint8_t party, uint8_t slot, uint8_t pp) {
	std::cout << "handle_battle_set_pp\n";
}

void BotClient::handle_battle_fainted (uint32_t field_id, uint8_t party, uint8_t slot, std::string const & nickname) {
	std::cout << "handle_battle_fainted\n";
}

void BotClient::handle_battle_begin_turn (uint32_t field_id, uint16_t turn_count) {
	std::cout << "handle_battle_begin_turn\n";
}

void BotClient::handle_battle_set_move (uint32_t field_id, uint8_t pokemon, uint8_t move_slot, uint16_t new_move, uint8_t pp, uint8_t max_pp) {
	std::cout << "handle_battle_set_move\n";
}

void BotClient::handle_metagame_list (std::vector <Metagame> const & metagames) {
	std::cout << "handle_metagame_list\n";
}

void BotClient::handle_invalid_team (std::vector <int16_t> const & violation) {
	std::cerr << "Invalid team.\n";
	for (std::vector<int16_t>::const_iterator it = violation.begin(); it != violation.end(); ++it) {
		int pokemon = (-(*it + 1)) % 6;
		std::cerr << "Problem at Pokemon " << pokemon << ", error code " << -(*it + pokemon + 1) / 6 << '\n';
	}
}

void BotClient::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	msg.send (socket);
}

void BotClient::accept_challenge (std::string const & user) {
	OutMessage msg (OutMessage::RESOLVE_CHALLENGE);
	msg.write_string (user);
	msg.write_byte (1);
	msg.write_team (battle.ai);
	msg.send (socket);
	std::cout << "Accepted challenge vs. " + user + "\n";
}


void BotClient::reject_challenge (std::string const & user) {
	OutMessage msg (OutMessage::RESOLVE_CHALLENGE);
	msg.write_string (user);
	msg.write_byte (0);
	msg.send (socket);
	std::cout << "Rejected challenge vs. " + user + "\n";
}

void BotClient::load_responses () {
	std::ifstream file ("responses.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line))
		response.push_back (line);
	file.close();
}

}		// namespace pl
}		// namespace technicalmachine
