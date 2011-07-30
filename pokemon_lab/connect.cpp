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
#include <iostream>
#include <netinet/in.h>
#include <queue>
#include <vector>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

#include "../crypt/md5.hpp"
#include "../crypt/sha2.hpp"
#include "../crypt/rijndael.h"


namespace technicalmachine {

class InMessage {
	private:
		std::vector <uint8_t> buffer;
		unsigned index;
	public:
		InMessage ();
		void recvfully (boost::asio::ip::tcp::socket & socket, unsigned bytes);
		uint8_t read_byte ();
		uint16_t read_short ();
		uint32_t read_int ();
		std::string read_string ();
	private:
		uint32_t read_bytes (int bytes);
	public:
		enum Message {
			WELCOME_MESSAGE = 0,
			PASSWORD_CHALLENGE = 1,
			REGISTRY_RESPONSE = 2,
			SERVER_INFO = 3,
			CHANNEL_INFO = 4,
			CHANNEL_JOIN_PART = 5,
			CHANNEL_STATUS = 6,
			CHANNEL_LIST = 7,
			CHANNEL_MESSAGE = 8,
			INCOMING_CHALLENGE = 9,
			FINALISE_CHALLENGE = 10,
			CHALLENGE_WITHDRAWN = 11,
			/** Battery of messages related to battles */
			BATTLE_BEGIN = 12,
			REQUEST_ACTION = 13,
			BATTLE_POKEMON = 14,
			BATTLE_PRINT = 15,
			BATTLE_VICTORY = 16,
			BATTLE_USE_MOVE = 17,
			BATTLE_WITHDRAW = 18,
			BATTLE_SEND_OUT = 19,
			BATTLE_HEALTH_CHANGE = 20,
			BATTLE_SET_PP = 21,
			BATTLE_FAINTED = 22,
			BATTLE_BEGIN_TURN = 23,
			SPECTATOR_BEGIN = 24,
			BATTLE_SET_MOVE = 25,
			METAGAME_LIST = 26,
			KICK_BAN_MESSAGE = 27,
			USER_DETAILS = 28,
			USER_MESSAGE = 29,
			BATTLE_STATUS_CHANGE = 30,
			CLAUSE_LIST = 31,
			INVALID_TEAM = 32,
			ERROR_MESSAGE = 33,
			PRIVATE_MESSAGE = 34,
			IMPORTANT_MESSAGE = 35
		};
};

InMessage::InMessage ():
	index (0) {
}

void InMessage::recvfully (boost::asio::ip::tcp::socket & socket, unsigned bytes) {
	buffer.clear();
	buffer.resize (bytes);
	index = 0;
	boost::asio::read (socket, boost::asio::buffer (buffer));
}

uint32_t InMessage::read_bytes (int bytes) {
	uint32_t data = 0;
	for (int n = 0; n != bytes; ++n) {
		data += buffer [index] << (8 * (bytes - n - 1));
		++index;
	}
	return data;
}

uint8_t InMessage::read_byte () {
	return read_bytes (1);
}

uint16_t InMessage::read_short () {
	return read_bytes (2);
}

uint32_t InMessage::read_int () {
	return read_bytes (4);
}

std::string InMessage::read_string () {
	unsigned short length = read_short ();
	std::cout << "length: " << length << '\n';
	std::string data = "";
	for (unsigned n = 0; n != length; ++n) {
		data += buffer [index];
		++index;
	}
	return data;
}

class OutMessage {
	private:
		std::vector <uint8_t> buffer;
	public:
		OutMessage (uint8_t code);
		void write_byte (uint8_t byte);
		void write_int (uint32_t bytes);
		void write_string (std::string const & string);
		void finalize (boost::asio::ip::tcp::socket & socket);
		enum Message {
			REQUEST_CHALLENGE = 0,
			CHALLENGE_RESPONSE = 1,
			REGISTER_ACCOUNT = 2,
			JOIN_CHANNEL = 3,
			CHANNEL_MESSAGE = 4,
			CHANNEL_MODE = 5,
			OUTGOING_CHALLENGE = 6,
			RESOLVE_CHALLENGE = 7,
			CHALLENGE_TEAM = 8,
			WITHDRAW_CHALLENGE = 9,
			BATTLE_ACTION = 10,
			PART_CHANNEL = 11,
			REQUEST_CHANNEL_LIST = 12,
			QUEUE_TEAM = 13,
			BAN_MESSAGE = 14,
			USER_INFO_MESSAGE = 15,
			USER_PERSONAL_MESSAGE = 16,
			USER_MESSAGE_REQUEST = 17,
			CLIENT_ACTIVITY = 18,
			CANCEL_QUEUE = 19,
			CANCEL_BATTLE_ACTION = 20,
			PRIVATE_MESSAGE = 21,
			IMPORTANT_MESSAGE = 22
		};
};

OutMessage::OutMessage (uint8_t code) {
	buffer.push_back (code);
}

void OutMessage::write_byte (uint8_t byte) {
	buffer.push_back (byte);
}

void OutMessage::write_int (uint32_t bytes) {
	uint32_t network_byte = htonl (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.push_back (*(byte + n));
}

void OutMessage::write_string (std::string const & string) {
	for (std::string::const_iterator it = string.begin(); it != string.end(); ++it)
		buffer.push_back (*it);
}

void OutMessage::finalize (boost::asio::ip::tcp::socket & socket) {
	uint32_t length = htonl (buffer.size() - 1);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.insert (buffer.begin() + 1, *(byte + n));
	boost::asio::write (socket, boost::asio::buffer (buffer));
}

class BotClient {
	private:
		std::string const username;
		std::string const password;
	public:
		boost::asio::io_service io;
		boost::asio::ip::tcp::socket socket;
		std::queue<OutMessage> send_queue;
		BotClient (std::string const &host, std::string const & port, std::string const & user, std::string const & pswd);
		void authenticate ();
		void send (OutMessage message);
		std::string get_shared_secret (int secret_style, std::string const & salt);
		std::string get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt);
		void run ();
		void handle_message (InMessage::Message &code, InMessage &msg);
		void handle_welcome_message (uint32_t version, std::string const & server, std::string const & message);
		void handle_challenge (InMessage msg);
		void handle_registry_response (uint8_t type, std::string const & details);
		void handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length);
		void join_channel (std::string const & channel);
//		void accept_challenge (std::string const & user, ??? team);
		void reject_challenge (std::string const & user);
};

BotClient::BotClient (std::string const & host, std::string const & port, std::string const & user, std::string const & pswd):
	username (user),
	password (pswd),
	socket (io) {
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
	send (message);
}

std::string BotClient::get_shared_secret (int secret_style, std::string const & salt) {
	if (secret_style == 0)
		return password;
	else if (secret_style == 1)
		return getMD5HexHash (password);
	else if (secret_style == 2)
		return getMD5HexHash (getMD5HexHash (password) + salt);
	else {
		std::cerr << "Unknown secret style of " << secret_style << '\n';
		return "";
	}	
}

std::string BotClient::get_challenge_response (std::string const & challenge, int secret_style, std::string const & salt) {
	std::string digest = getSHA256Hash (get_shared_secret (secret_style, salt));
	rijndael_ctx ctx;
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 16);
	unsigned char middle [16];
	rijndael_decrypt (&ctx, reinterpret_cast <unsigned char const *> (challenge.c_str()), middle);
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 16);
	unsigned char result [16];
	rijndael_decrypt (&ctx, middle, result);

	uint32_t r = (result [0] << 3) + (result [1] << 2) + (result [3] << 1) + result [4] + 1;
	unsigned char response_array [16] = { 0 };
	uint8_t * byte = reinterpret_cast <uint8_t *> (&r);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		response_array [n] = (*(byte + n));
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (0, 16).c_str()), 16);
	rijndael_encrypt (&ctx, response_array, middle);
	rijndael_set_key (&ctx, reinterpret_cast <unsigned char const *> (digest.substr (16, 16).c_str()), 16);
	rijndael_encrypt (&ctx, middle, response_array);
	std::string response = "";
	for (unsigned n = 0; n != 16; ++n)
		response += response_array [n];
	return response;
}

void BotClient::send (OutMessage message) {
	send_queue.push (message);
}

void activity_proxy (BotClient & client) {
	while (true) {
		boost::asio::deadline_timer timer (client.io, boost::posix_time::seconds (45));
		timer.wait ();
		client.send (OutMessage (OutMessage::CLIENT_ACTIVITY));
	}
}

void send_proxy (BotClient & client) {
	while (true) {
		OutMessage msg = client.send_queue.front();
		client.send_queue.pop();
		msg.finalize (client.socket);
	}
}

void BotClient::run () {
	while (true) {
		std::cout << "first\n";
		InMessage msg;
		// read in the five byte header
		msg.recvfully (socket, 5);

		// extract the message type and length components
		InMessage::Message code = static_cast <InMessage::Message> (msg.read_byte ());
		std::cout << "code: " << code << '\n';
		uint32_t length = msg.read_int ();
		std::cout << "length: " << length << '\n';

		// read in the whole message
		msg.recvfully (socket, length);
		std::cout << "Hey\n";
		handle_message (code, msg);
		std::cout << "Oh yeah!\n";
	}
}

void BotClient::handle_message (InMessage::Message & code, InMessage & msg) {
	switch (code) {
		case InMessage::WELCOME_MESSAGE: {
			// int32  : server version
			// string : server name
			// string : welcome message
			uint32_t version = msg.read_int();
			std::string server = msg.read_string();
			std::string message = msg.read_string();
			handle_welcome_message (version, server, message);
			break;
		}
		case InMessage::PASSWORD_CHALLENGE:
			handle_challenge (msg);
			break;
		case InMessage::REGISTRY_RESPONSE:
			// byte   : type
			// string : details
			handle_registry_response (msg.read_byte(), msg.read_string());
			break;
		case InMessage::CHANNEL_INFO:
			// _int 32 : channel id
			// byte   : channel info
			// string : channel name
			// string : channel topic
			// int32  : channel flags
			// int32  : number of users
			// for each user:
			//   string : name
			//   int32  : flags
/*			uint32_t id = msg.read_int();
			uint8_t info = msg.read_byte();
			std::string name = msg.read_string();
			std::string topic = msg.read_string();
			uint32_t flags = msg.read_int();
			uint32_t pop = msg.read_int();
			users = [(msg.read_string(), msg.read_int()) for i in range(pop)]
			handle_channel_info (id, info, name, topic, flags, users);
*/			break;
		case InMessage::CHANNEL_JOIN_PART:
			// int32  : channel id
			// string : user
			// byte   : joining?
//			handle_channel_join_part (msg.read_int(), msg.read_string(), msg.read_byte())
			break;
		case InMessage::CHANNEL_STATUS:
			// int32  : channel id
			// string: invoker
			// string : user
			// int32  : flags
//			handle_channel_status (msg.read_int(), msg.read_string(), msg.read_string(), msg.read_int())
			break;
		case InMessage::CHANNEL_LIST:
			// int32 : number of channels
			// for each channel:
			//   string : name
			//   byte   : type
			//   string : topic
			//   int32 : population
/*			uint32_t n = msg.read_int()
			channels = [(msg.read_string(), msg.read_byte(), msg.read_string(), msg.read_int()) for i in range(n)]
			handle_channel_list (channels)
*/			break;
		case InMessage::CHANNEL_MESSAGE:
			// int32  : channel id
			// string : user
			// string : message
//			handle_channel_message (msg.read_int(), msg.read_string(), msg.read_string())
			break;
		case InMessage::INCOMING_CHALLENGE:
			// string : user
			// byte   : generation
			// int32  : active party size
			// int32  : max team length
			handle_incoming_challenge (msg.read_string(), msg.read_byte(), msg.read_int(), msg.read_int());
			break;
		case InMessage::FINALISE_CHALLENGE:
			// string : user
			// byte   : whether the challenge was accepted
//			handle_finalize_challenge (msg.read_string(), msg.read_byte())
			break;
		case InMessage::BATTLE_BEGIN:
			// int32  : field id
			// string : opponent
			// byte   : party
//			handle_battle_begin (msg.read_int(), msg.read_string(), msg.read_byte())
			break;
		case InMessage::REQUEST_ACTION:
			// int32  : field id
			// byte   : slot of pokemon
			// byte   : position of pokemon
			// byte   : whether this is a replacement
			// int32  : number of pokemon
			// for each pokemon
			//	 byte: whether it is legal to switch to this pokemon
			// if not replacement
			//	 byte : whether switching is legal
			//	 byte : whether there is a forced move
			//	 if not forced:
			//		 int32 : total number of moves
			//		 for each move:
			//			 byte: whether the move is legal
/*			fid = msg.read_int();
			slot = msg.read_byte();
			pos = msg.read_byte();
			replace = msg.read_byte();
			num_pokes = msg.read_int();
			switches = [msg.read_byte() for i in range(num_pokes)]
			can_switch = false;
			forced = false;
			moves = []
			if (replace == 0) {
				can_switch = msg.read_byte()
				forced = msg.read_byte()
				if forced == 0:
					num_moves = msg.read_int()
					moves = [msg.read_byte() for i in range(num_moves)]
			}
			if not 1 in switches:
				can_switch = false
			handle_request_action(fid, slot, pos, replace, switches, can_switch, forced, moves)
*/			break;
		case InMessage::BATTLE_POKEMON:
			// int32 : field id
			// for 0..1:
			//	 for 0..n-1:
			//		 int16 : species id
			//		 if id != -1:
			//			 byte: gender
			//			 byte: level
			//			 byte: shiny
		   
			// the bot probably doesn't need to care about this
			break;
		case InMessage::BATTLE_PRINT:
			// int32 : field id
			// byte  : category
			// int16 : message id
			// byte  : number of arguments
			// for each argument:
			//   string : value of the argument
/*			fid = msg.read_int();
			cat = msg.read_byte();
			id = msg.read_short();
			argc = msg.read_byte();
			args = [msg.read_string() for i in range(argc)]
			handle_battle_print (fid, cat, id, args);
*/			break;
		case InMessage::BATTLE_VICTORY:
			// int32 : field id
			// int16 : party id
//			handle_battle_victory (msg.read_int(), msg.read_short());
			break;
		case InMessage::BATTLE_USE_MOVE:
			// int32  : field id
			// byte   : party
			// byte   : slot
			// string : user [nick]name
			// int16  : move id
//			handle_battle_use_move (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_string(), msg.read_short());
			break;
		case InMessage::BATTLE_WITHDRAW:
			// int32  : field id
			// byte   : party
			// byte   : slot
			// string : user [nick]name
//			handle_battle_withdraw (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_string());
			break;
		case InMessage::BATTLE_SEND_OUT:
			// int32  : field id
			// byte   : party
			// byte   : slot
			// byte   : index
			// string : user [nick]name
			// int16  : species id
			// byte   : gender
			// byte   : level
//			handle_battle_send_out (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_byte(), msg.read_string(), msg.read_short(), msg.read_byte(), msg.read_byte());
			break;
		case InMessage::BATTLE_HEALTH_CHANGE:
			// int32 : field id
			// byte  : party
			// byte  : slot
			// int16 : delta health in [0, 48]
			// int16 : new total health in [0, 48]
			// int16 : denominator
//			handle_battle_health_change (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_short(), msg.read_short(), msg.read_short());
			break;
		case InMessage::BATTLE_SET_PP:
			// int32 : field id
			// byte  : party
			// byte  : slot
			// byte  : pp
//			handle_battle_set_pp (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_byte());
			break;
		case InMessage::BATTLE_FAINTED:
			// int32  : field id
			// byte   : party
			// byte   : slot
			// string : user [nick]name
//			handle_battle_fainted (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_string());
			break;
		case InMessage::BATTLE_BEGIN_TURN:
			// int32 : field id
			// int16 : turn count
//			handle_battle_begin_turn (msg.read_int(), msg.read_short());
			break;
		case InMessage::SPECTATOR_BEGIN:
			break;
		case InMessage::BATTLE_SET_MOVE:
			// int32  : field id
			// byte   : pokemon
			// byte   : move slot
			// int16  : new move
			// byte   : pp
			// byte   : max pp
//			handle_battle_set_move (msg.read_int(), msg.read_byte(), msg.read_byte(), msg.read_short(), msg.read_byte(), msg.read_byte());
			break;
		case InMessage::METAGAME_LIST:
			// int16  : metagame count
			// for 0..metagame count - 1:
			//	byte   : index
			//	string : name
			//	string : id
			//	string : description
			//	byte   : party size
			//	byte   : max team length
			//	int16  : ban list count
			//	for 0..ban list count - 1:
			//		int16  : species id
			//	int16  : clause count
			//	for 0..clause count - 1:
			//		string : clause name
			//	byte	: if timing is enabled
			//	if timing is enabled:
			//	   short : pool length
			//	   byte  : periods
			//	   short : period length
/*			mcount = msg.read_short()
			metagames = []
			for i in xrange(0, mcount):
				index, name, id, desc = msg.read_byte(), msg.read_string(), msg.read_string(), msg.read_string()
				party_size, max_team_length = msg.read_byte(), msg.read_byte()
				bans = [msg.read_short() for i in xrange(0, msg.read_short())]
				clauses = [msg.read_string() for i in xrange(0, msg.read_short())]
				timing = (msg.read_byte() != 0)
				if timing:
					pool = msg.read_short();
					periods = msg.read_byte();
					period_length = msg.read_short();
				else:
					pool = -1;
					periods = -1;
					period_length = -1
				metagames.append((index, name, id, desc, party_size, max_team_length, bans, clauses, pool, periods, period_length))
			handle_metagame_list(metagames)
*/			break;
		default:
			std::cerr << "Unknown code: " << code << '\n';
			break;
	}
}

void BotClient::handle_welcome_message (uint32_t version, std::string const & server, std::string const & message) {
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
	send (out);
}

void BotClient::handle_registry_response (uint8_t type, std::string const & details) {
	if (type == 7) {
		std::cout << "Successfully authenticated!\n";
		join_channel ("main");
	}
	else {
		std::cout << "Authentication failed with code: " << type << ". =(\n";
		if (details.length() > 0)
			std::cout << details + "\n";
	}
}

void BotClient::handle_incoming_challenge (std::string const & user, uint8_t generation, uint32_t n, uint32_t team_length) {
	if (true or (n > 1 or team_length > 6 or generation != 4))
		reject_challenge (user);
	else {
//		accept_challenge (user, team);
	}
}

void BotClient::join_channel (std::string const & channel) {
	OutMessage msg (OutMessage::JOIN_CHANNEL);
	msg.write_string (channel);
	send (msg);
}

/* void BotClient::accept_challenge (std::string const & user, ??? team) {
	OutMessage msg (7);
	msg.write_string (user);
	msg.write_byte (1);
	write_team (msg, team);
	send (msg);
}
*/

void BotClient::reject_challenge (std::string const & user) {
	OutMessage msg (OutMessage::RESOLVE_CHALLENGE);
	msg.write_string (user);
	msg.write_byte (0);
	send (msg);
}

}
using namespace technicalmachine;

int main () {
	std::string const host = "lab.pokemonexperte.de";
	std::string const port = "8446";
	std::string const username = "TM1.0";
	std::string const password = "Maximum Security";
	BotClient client (host, port, username, password);
	boost::thread keep_alive (boost::bind (& activity_proxy, boost::ref (client)));
	boost::thread send_messages (boost::bind (& send_proxy, boost::ref (client)));
	client.authenticate ();
	std::cout << "Authenticated.\n";
	client.run();
	return 0;
}
