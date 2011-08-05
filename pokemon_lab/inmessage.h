// Pokemon Lab incoming messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_LAB_INMESSAGE_H_
#define POKEMON_LAB_INMESSAGE_H_

#include <cstdint>
#include <vector>
#include "../species.h"
#include <boost/asio.hpp>
#undef SING

namespace technicalmachine {
namespace pl {

class BotClient;

class InMessage {
	private:
		std::vector <uint8_t> buffer;
		unsigned index;
	public:
		InMessage ();
		void reset (unsigned bytes);
		void read_header (boost::asio::ip::tcp::socket & socket, BotClient * client);
		void read_body (boost::asio::ip::tcp::socket & socket, BotClient * client);
		uint8_t read_byte ();
		uint16_t read_short ();
		uint32_t read_int ();
		std::string read_string ();
		void skip ();
	private:
		uint32_t read_bytes (int bytes);
	public:
		static species pl_to_tm_species (int id);
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
			FINALIZE_CHALLENGE = 10,
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

}		// namespace pl
}		// namespace technicalmachine
#endif
