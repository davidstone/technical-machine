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

Battle::Battle (Map const & map, std::string const & opponent, int depth_):
	ai (true, map, 6),
	foe (false, map, ai.size),
	log (ai, foe),
	depth (depth_) {
	for (std::vector <Pokemon>::const_iterator pokemon = ai.pokemon.set.begin(); pokemon != ai.pokemon.set.end(); ++pokemon)
		slot_memory.push_back (pokemon->name);
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
	std::ifstream file ("settings.txt");
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
		case InMessage::CHALLENGE_WITHDRAWN: {
			std::string opponent = msg.read_string ();
			challenges.erase (opponent);
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
		   
			// the bot probably doesn't need to care about this
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
			Battle & battle = battles.find (field_id)->second;
			battle.handle_print (category, message_id, arguments);
			break;
		}
		case InMessage::BATTLE_VICTORY: {
			uint32_t field_id = msg.read_int ();
			uint16_t party_id = msg.read_short ();
			handle_victory (field_id, party_id);
			break;
		}
		case InMessage::BATTLE_USE_MOVE: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			uint16_t move_id = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_use_move (party, slot, nickname, move_id);
			break;
		}
		case InMessage::BATTLE_WITHDRAW: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_withdraw (party, slot, nickname);
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
			Battle & battle = battles.find (field_id)->second;
			battle.handle_send_out (map, party, slot, index, nickname, species_id, gender, level);
			break;
		}
		case InMessage::BATTLE_HEALTH_CHANGE: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			int16_t change_in_health = msg.read_short ();
			int16_t remaining_health = msg.read_short ();
			uint16_t denominator = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_health_change (party, slot, change_in_health, remaining_health, denominator);
			break;
		}
		case InMessage::BATTLE_SET_PP: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			uint8_t pp = msg.read_byte ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_set_pp (party, slot, pp);
			break;
		}
		case InMessage::BATTLE_FAINTED: {
			uint32_t field_id = msg.read_int ();
			uint8_t party = msg.read_byte ();
			uint8_t slot = msg.read_byte ();
			std::string nickname = msg.read_string ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_fainted (party, slot, nickname);
			break;
		}
		case InMessage::BATTLE_BEGIN_TURN: {
			uint32_t field_id = msg.read_int ();
			uint16_t turn_count = msg.read_short ();
			Battle & battle = battles.find (field_id)->second;
			battle.handle_begin_turn (turn_count);
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
			Battle & battle = battles.find (field_id)->second;
			battle.handle_set_move (pokemon, move_slot, new_move, pp, max_pp);
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
		case InMessage::KICK_BAN_MESSAGE: {
			msg.skip ();
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
	if (type == 7) {
		join_channel ("main");
		send_battle_challenge ();
	}
	else {
		std::cerr << "Authentication failed with code: " << static_cast <int> (type) << ". =(\n";
		if (details.length() > 0)
			std::cerr << details + "\n";
	}
}

void BotClient::handle_channel_info (uint32_t id, uint8_t info, std::string const & channel_name, std::string const & topic, uint32_t channel_flags, std::vector <std::pair <std::string, uint32_t> > const & users) {
}

void BotClient::handle_channel_join_part (uint32_t id, std::string const & user, bool joining) {
}

void BotClient::handle_channel_status (uint32_t channel_id, std::string const & invoker, std::string const & user, uint32_t flags) {
}

void BotClient::handle_channel_list (std::vector <Channel> const & channels) {
}

void BotClient::handle_channel_message (uint32_t channel_id, std::string const & user, std::string const & message) {
	// Vanity mode!
	std::string msg = message;
	boost::to_lower (msg);
	if (msg.find ("obi") != std::string::npos or msg.find ("david stone") != std::string::npos or msg.find ("technical machine") != std::string::npos or msg.find ("tm") != std::string::npos)
		std::cout << message + "\n"; 
}

void BotClient::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	msg.send (socket);
}

void BotClient::send_battle_challenge () {
	OutMessage msg (OutMessage::OUTGOING_CHALLENGE);
	std::cout << "Enter opponent's name (blank to be challenged): ";
	std::string opponent;
	getline (std::cin, opponent);
	if (opponent.empty ())
		challenger = false;
	else {
		challenger = true;
		uint8_t const generation = 1;
		uint32_t const party_size = 1;
		uint32_t const team_length = 6;
		msg.write_challenge (opponent, generation, party_size, team_length);
		msg.send (socket);
	}
}

void BotClient::handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length) {
	bool accepted;
	if (n > 1 or team_length != 6 or (user != "Technical Machine" and user != "david stone"))
		accepted = false;
	else
		accepted = true;
	handle_finalize_challenge (user, accepted);
}

void BotClient::handle_finalize_challenge (std::string const & opponent, bool accepted) {
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
		Battle battle (map, opponent, depth);
		challenges.insert (std::pair <std::string, Battle> (opponent, battle));
		msg.write_team (battle.ai);
		verb = "Accepted";
	}
	else
		verb = "Rejected";
	msg.send (socket);
	std::cout << verb + " challenge vs. " + opponent + "\n";
}

void BotClient::handle_battle_begin (uint32_t field_id, std::string const & opponent, uint8_t party) {
 	Battle & battle = challenges.find (opponent)->second;
	battle.party = party;
	battles.insert (std::pair <uint32_t, Battle> (field_id, battle));
	challenges.erase (opponent);
	boost::asio::deadline_timer timer (io, boost::posix_time::seconds(15));
	timer.wait ();
}

void Battle::handle_request_action (BotClient & botclient, uint32_t field_id, uint8_t slot, uint8_t index, bool replace, std::vector <uint8_t> const & switches, bool can_switch, bool forced, std::vector <uint8_t> const & moves) {
	do_turn (*log.first, *log.last, weather);
	incorrect_hp (*log.first);
	incorrect_hp (*log.last);
	if (rand () % 50 == 0) {
		OutMessage comment (OutMessage::CHANNEL_MESSAGE);
		comment.write_int (field_id);
		comment.write_string (botclient.get_response ());
		comment.send (botclient.socket);
	}
	OutMessage msg (OutMessage::BATTLE_ACTION);
	if (forced)
		msg.write_move (field_id, 1);
	else {
		Team predicted = foe;
		std::cout << "======================\nPredicting...\n";
		predict_team (botclient.detailed, predicted, ai.size);
		std::string out;
		predicted.output (out);
		std::cout << out;

		int64_t score;
		moves_list move = expectiminimax (ai, predicted, weather, depth, botclient.sv, score);
		if (Move::is_switch (move))
			msg.write_switch (field_id, switch_slot (move));
		else {
			uint8_t move_index = 0;
			while (ai.pokemon->move.set [move_index].name != move)
				++move_index;
			msg.write_move (field_id, move_index, 1 - party);
		}
	}
	msg.send (botclient.socket);
	if (!ai.replacing)
		log.initialize_turn (ai, foe);
}

void Battle::handle_print (uint8_t category, uint16_t message_id, std::vector <std::string> const & arguments) {
	std::cout << "party id: " << static_cast <int> (party) << '\n';
	std::cout << "category: " << static_cast <int> (category) << '\n';
	std::cout << "message_id: " << message_id << '\n';
	for (std::vector <std::string>::const_iterator it = arguments.begin(); it != arguments.end(); ++it)
		std::cout << "\t" + *it + '\n';
	switch (category) {
		case InMessage::BATTLE_MESSAGES:
			switch (message_id) {
				case 2:
				case 6:
					log.active->miss = true;
					break;
				case 9:
					log.active->ch = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_STAT_LEVEL:
			break;
		case InMessage::STATUS_EFFECTS_BURN:
			if (message_id == 1)
				log.active->at_replacement().move->variable.index = 1;
			break;
		case InMessage::STATUS_EFFECTS_CONFUSION:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
				case 2:	// Confusion ends
					break;
				case 4:
					log.active->hitself = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_FREEZE:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
				case 2:	// Defrost
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_PARALYSIS:
			switch (message_id) {
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					std::cout << "Me: " << log.active->me << '\n';
					break;
				case 2:
					log.active->fully_paralyzed = true;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_POISON:
			switch (message_id) {
				case 1:
				case 2:
					log.active->at_replacement().move->variable.index = 1;
					break;
			}
			break;
		case InMessage::STATUS_EFFECTS_SLEEP:
			case 1:
				log.active->at_replacement().move->variable.index = 1;
				break;
			case 2:		// Woke up
				break;
			break;
		case InMessage::STATUS_EFFECTS_FLINCH:
			switch (message_id)
				case 1:
					log.active->at_replacement().move->variable.index = 1;
					break;
			break;
		case InMessage::STATUS_EFFECTS_ATTRACT:
			break;
		// Weather messages need to be parsed in case of items that extend their effects
		case InMessage::WEATHER_HAIL:
			break;
		case InMessage::WEATHER_RAIN:
			break;
		case InMessage::WEATHER_SANDSTORM:
			break;
		case InMessage::WEATHER_SUN:
			break;
		case InMessage::BATTLE_MESSAGES_UNIQUE:
			switch (message_id) {
				case 0:		// Multi-hit moves
					break;
				case 1:		// $1 was cured of $2!
					break;
				case 2:		// But nothing happened!
					break;
				case 7:		// Parse this message to get base attack of each Pokemon (Beat Up)
					break;
				case 9:		// $1 became the $2 type!
					break;
				case 10:		// $1 stole $2's $3!
					break;
				case 11:		// Could not steal the $1 from $2!
					break;
				case 12:		// $1 knocked off the foe $2's $3!
					break;
				case 13:		// $1 obtained $2! (trick)
					break;
				case 14:		// Magnitude $1!
					break;
				case 18:		// $1 was released!
					break;		// Determine whether Grip Claw was used
				case 22:		// $1 is tightening its focus! Can be useful with BP and U-turn
					break;
				case 26:		// $1's taunt wore off
					break;
				case 31:		// $1 consumed $2's $3 (Bug Bite)
				case 147:	// $1 used $2's $3 (Pluck)
					break;
				case 37:		// $1 learned $2!
					break;
				case 58:		// $1 was hit by recoil!
					break;
				case 70:		// $1's $2 was disabled!
					break;
				case 80:		// $1 wore off! (Reflect / Light Screen)
					break;
				case 97:		// $1 became confused due to fatigue (Thrash clones)	
					break;
				case 136:	// $1's substitute faded
					break;
				case 146:	// $1 got a $2! (Recycle)
					break;
				case 153:	// $1 calmed down (Uproar)
					break;
				case 156:	// $1's encore ended.
					break;
				case 157:	// $1 flung its $2!
					break;
			}
			break;
		case InMessage::ABILITY_MESSAGES:
			switch (message_id) {
				case 0:
					log.inactive->at_replacement().ability = AFTERMATH;
					break;
				case 1:
					log.inactive->at_replacement().ability = ANGER_POINT;
					break;
				case 2:
					log.active->at_replacement().ability = ANTICIPATION;	// fix
					break;
				case 3:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = BAD_DREAMS;
					break;
				case 4:
					log.inactive->at_replacement().ability = CLEAR_BODY;
//					log.active->at_replacement().ability = HYPER_CUTTER;
					break;
				case 5:
					log.inactive->at_replacement().ability = CUTE_CHARM;
					break;
				case 6:
					log.inactive->at_replacement().ability = DAMP;
					break;
				case 7:
					log.active->at_replacement().ability = DOWNLOAD;
					break;
				case 8:
					log.active->at_replacement().ability = DRIZZLE;
					break;
				case 9:
					log.active->at_replacement().ability = DROUGHT;
					break;
				case 10:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = DRY_SKIN;
					break;
				case 11:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = DRY_SKIN;
//					log.active->at_replacement().ability = SOLAR_POWER;
					break;
				case 12:
					log.inactive->at_replacement().ability = DRY_SKIN;
					break;
				case 14:
					log.inactive->at_replacement().ability = FLAME_BODY;
					break;
				case 15:
					log.inactive->at_replacement().ability = FLASH_FIRE;
					break;
				case 16:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = FLOWER_GIFT;
					break;
				case 17:		// $1 Forewarn alerted it to $2!
					log.active->at_replacement().ability = FOREWARN;
					break;
				case 18:		// $1 found $2's $3!
					log.active->at_replacement().ability = FRISK;
					break;
				case 19:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = HYDRATION;
					break;
				case 20:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = ICE_BODY;
					break;
				case 21:
					log.inactive->at_replacement().ability = IMMUNITY;
					break;
				case 22:
					log.inactive->at_replacement().ability = INSOMNIA;
					break;
				case 23:
					log.active->at_replacement().ability = INTIMIDATE;
					break;
				case 24:
					log.inactive->at_replacement().ability = LEAF_GUARD;
					break;
				case 25:
					log.inactive->at_replacement().ability = LEVITATE;
					break;
				case 26:		// $1's $2 drew the attack!
					log.inactive->at_replacement().ability = LIGHTNINGROD;
//					log.active->at_replacement().ability = STORM_DRAIN;
					break;
				case 27:
					log.inactive->at_replacement().ability = LIMBER;
					break;
				case 28:
					log.active->at_replacement().ability = MOLD_BREAKER;
					break;
				case 29:
					log.inactive->at_replacement().ability = MOTOR_DRIVE;
					break;
				case 30:
					log.inactive->at_replacement().ability = OBLIVIOUS;
					break;
				case 31:
					log.inactive->at_replacement().ability = OWN_TEMPO;
					break;
				case 32:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = POISON_HEAL;
					break;
				case 34:
					log.active->at_replacement().ability = PRESSURE;
					break;
				case 35:
					log.active->at_replacement().ability = QUICK_FEET;
					break;
				case 36:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = RAIN_DISH;
					break;
				case 37:
					log.inactive->at_replacement().ability = ROUGH_SKIN;
					break;
				case 38:
					log.active->at_replacement().ability = SAND_STREAM;
					break;
				case 39:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = SHED_SKIN;
					break;
				case 40:
					log.active->at_replacement().ability = SLOW_START;
					break;
				case 41:
					log.active->at_replacement().ability = SNOW_WARNING;
					break;
				case 42:
					log.inactive->at_replacement().ability = SOUNDPROOF;
					break;
				case 43:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = SPEED_BOOST;
					break;
				case 45:
					log.inactive->at_replacement().ability = STEADFAST;
					break;
				case 46:
					log.inactive->at_replacement().ability = STICKY_HOLD;
					break;
				case 47:
					log.inactive->at_replacement().ability = STURDY;
					break;
				case 48:
					log.active->at_replacement().ability = SYNCHRONIZE;		// Fix
					break;
				case 49:
					log.inactive->at_replacement().ability = TANGLED_FEET;
					break;
				case 50:		// $1 traced $2's $3!
					log.active->at_replacement().ability = TRACE;
//					log.inactive->at_replacement().ability = ;
					break;
				case 51:
					log.active->at_replacement().ability = TRUANT;
					break;
				case 52:
					log.active->at_replacement().ability = UNBURDEN;
					break;
				case 53:
					log.inactive->at_replacement().ability = WATER_VEIL;
					break;
				case 54:
					log.inactive->at_replacement().ability = WONDER_GUARD;
					break;
				case 55:
					log.inactive->at_replacement().ability = LIQUID_OOZE;
					break;
				case 56:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = ICE_BODY;
					break;
				case 57:
					update_active_print (log, arguments);
					log.active->at_replacement().ability = DRY_SKIN;
					break;
				case 59:		// $1's $2 raised its $3!
//					log.active->at_replacement().ability = 
					break;
				case 60:		// $1's $2 paralyzed $3!
//					log.active->at_replacement().ability = 
					break;
				case 61:		// $1's $2 poisoned $3!
//					log.active->at_replacement().ability = 
					break;
				case 62:		// $1's $2 made $3 fall asleep!
//					log.active->at_replacement().ability = 
					break;
			}
			break;
		case InMessage::ITEM_MESSAGES:
			switch (message_id) {
				case 0:		// $1's $2 restored its health a little!
					update_active_print (log, arguments);
					std::cout << log.active->at_replacement().get_name() + " has Leftovers.\n";
					log.active->at_replacement().item = LEFTOVERS;
					break;
				case 1:		// $1's $2 cured its $3!
//					log.active->at_replacement().item = 
					break;
				case 2:
					log.active->at_replacement().item = QUICK_CLAW;
					break;
				case 3:		// $1's $2 raised its $3!
//					log.active->at_replacement().item = 
					break;
				case 4:		// The berry was the wrong flavor for $1!
//					log.active->at_replacement().item = 
					break;
				case 5:		// #1's $2 weakened $3's power!
//					log.active->at_replacement().item = 
					break;
				case 6:		// $1 was hurt by $2's $3!
//					log.active->at_replacement().item = 
					break;
				case 7:		// Fix this
					log.active->at_replacement().item = STICKY_BARB;
					break;
				case 8:
					update_active_print (log, arguments);
					log.active->at_replacement().item = BLACK_SLUDGE;
					break;
				case 9:		// The $1 latched on to $2!
//					log.active->at_replacement().item = 
					break;
				case 10:
					log.inactive->at_replacement().item = FOCUS_SASH;
					break;
				case 11:
					log.active->at_replacement().item = CUSTAP_BERRY;
					break;
				case 12:
					log.active->at_replacement().item = POWER_HERB;
					break;
			}
			break;
		default:
			break;
	}
}

void Battle::update_active_print (Log & log, std::vector <std::string> const & arguments) {
	// This needs to be changed to the correct message parser.
	std::cout << "arguments.size (): " << arguments.size () << '\n';
	std::cout << "arguments [0].length (): " << arguments [0].length () << '\n';
	assert (arguments.size() > 0);
	if (arguments [0] [3] - '0' == party) {
		log.active = &ai;
		log.inactive = &foe;
	}
	else {
		log.active = &foe;
		log.inactive = &ai;
	}
}

void BotClient::handle_victory (uint32_t field_id, uint16_t party_id) {
	std::string verb;
	Battle & battle = battles.find (field_id)->second;
	if (battle.party == party_id)
		verb = "Won";
	else
		verb = "Lost";
	std::cout << verb + " a battle vs. " + battle.foe.player + "\n";
	battles.erase (field_id);
}

void Battle::handle_use_move (uint8_t party_, uint8_t slot, std::string const & nickname, uint16_t move_id) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = &ai;
		other = &foe;
	}
	else {
		team = &foe;
		other = &ai;
	}
	log.active = team;
	log.inactive = other;
	if (log.first == NULL) {
		log.first = team;
		log.last = other;
	}
	int move = move_id;
	if (move >= SWITCH0)
		move += 6;
	log.log_move (static_cast <moves_list> (move));
}

void Battle::handle_withdraw (uint8_t party, uint8_t slot, std::string const & nickname) {
}

void Battle::handle_send_out (Map const & map, uint8_t party_, uint8_t slot, uint8_t index, std::string const & nickname, uint16_t species_id, uint8_t gender, uint8_t level) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = &ai;
		other = &foe;
	}
	else {
		team = &foe;
		other = &ai;
	}
	species name = InMessage::pl_to_tm_species (species_id);
	log.pokemon_sent_out (map, name, nickname, level, static_cast <genders> (gender), *team, *other);
}

void Battle::handle_health_change (uint8_t party_id, uint8_t slot, int16_t change_in_health, int16_t remaining_health, uint16_t denominator) {
	std::cout << "change_in_health: " << change_in_health << '\n';
	std::cout << "remaining_health: " << remaining_health << '\n';
	if (log.move_damage) {
		int type1 = effectiveness [log.active->at_replacement().move->type] [log.inactive->at_replacement ().type1];
		int type2 = effectiveness [log.active->at_replacement().move->type] [log.inactive->at_replacement ().type2];
		if ((type1 * type2 > 0) and (GROUND != log.active->at_replacement().move->type or grounded (*log.inactive, weather))) {
			log.inactive->damage = log.inactive->at_replacement().hp.max * change_in_health / denominator;
			if (static_cast <unsigned> (log.inactive->damage) > log.inactive->at_replacement().hp.stat)
				log.inactive->damage = log.inactive->at_replacement().hp.stat;
		}
		log.move_damage = false;
	}
	
	if (remaining_health < 0)
		remaining_health = 0;
	// If the message is about me, active must be me, otherwise, active must not be me
	if ((party_id == party) == log.active->me) {
		std::cout << "Updating active HP\n";
		log.active->at_replacement().new_hp = remaining_health;
	}
	else {
		std::cout << "Updating inactive HP\n";
		log.inactive->at_replacement().new_hp = remaining_health;
	}
}

void Battle::handle_set_pp (uint8_t party_, uint8_t slot, uint8_t pp) {
}

void Battle::handle_fainted (uint8_t party_, uint8_t slot, std::string const & nickname) {
	Team * team;
	Team * other;
	if (party == party_) {
		team = &ai;
		other = &foe;
	}
	else {
		team = &foe;
		other = &ai;
	}
	team->at_replacement().fainted = true;
}

void Battle::handle_begin_turn (uint16_t turn_count) {
	std::cout << "Begin turn " << turn_count << '\n';
}

void Battle::handle_set_move (uint8_t pokemon, uint8_t move_slot, uint16_t new_move, uint8_t pp, uint8_t max_pp) {
}

uint8_t Battle::switch_slot (moves_list move) {
	uint8_t slot = move - SWITCH0;
	for (uint8_t n = 0; n != slot_memory.size(); ++n) {
		if (slot_memory [n] == ai.pokemon.set [slot].name)
			return n;
	}
	assert (false);
	return 0;		// This should never happen
}

void BotClient::handle_metagame_list (std::vector <Metagame> const & metagames) {
}

void BotClient::handle_invalid_team (std::vector <int16_t> const & violation) {
	std::cerr << "Invalid team.\n";
	for (std::vector<int16_t>::const_iterator it = violation.begin(); it != violation.end(); ++it) {
		int pokemon = (-(*it + 1)) % 6;
		std::cerr << "Problem at Pokemon " << pokemon << ", error code " << -(*it + pokemon + 1) / 6 << '\n';
	}
}

void Battle::incorrect_hp (Team & team) {
	unsigned max_hp = 48;
	for (std::vector<Pokemon>::iterator pokemon = team.pokemon.set.begin(); pokemon != team.pokemon.set.end(); ++pokemon) {
		if (team.me)
			max_hp = pokemon->hp.max;
		unsigned pixels = max_hp * pokemon->hp.stat / pokemon->hp.max;
		if (pixels != pokemon->new_hp and (pokemon->new_hp - 1U > pixels or pixels > pokemon->new_hp + 1U)) {
			std::cerr << "Uh oh! " + pokemon->get_name () + " has the wrong HP! Pokemon Lab reports approximately " << pokemon->new_hp * pokemon->hp.max / max_hp << " but TM thinks it has " << pokemon->hp.stat << "\n";
			pokemon->hp.stat = pokemon->new_hp * pokemon->hp.max / max_hp;
		}
	}
}

void BotClient::load_responses () {
	std::ifstream file ("responses.txt");
	std::string line;
	for (getline (file, line); !file.eof(); getline (file, line))
		response.push_back (line);
	file.close();
}

std::string BotClient::get_response () const {
	return response [rand() % response.size()];
}

}		// namespace pl
}		// namespace technicalmachine
