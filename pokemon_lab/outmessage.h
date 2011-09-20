// Pokemon Lab outgoing messages
// Copyright 2011 David Stone
//
// This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License
// as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef POKEMON_LAB_OUTMESSAGE_H_
#define POKEMON_LAB_OUTMESSAGE_H_

#include <cstdint>
#include <string>
#include <vector>
#include <boost/asio.hpp>

#include "../species.h"

namespace technicalmachine {
class Team;
namespace pl {

class OutMessage {
	public:
		std::vector <uint8_t> buffer;
		OutMessage (uint8_t code);
		void write_byte (uint8_t byte);
		void write_short (uint16_t bytes);
		void write_int (uint32_t bytes);
		void write_string (std::string const & string);
		void write_team (Team const & team);
		void write_move (uint32_t field_id, uint8_t move_index, uint8_t target = 1);
		void write_switch (uint32_t field_id, uint8_t slot);
		void write_challenge (std::string const & opponent, uint8_t generation, uint32_t party_size, uint32_t team_length, uint32_t metagame = 0, std::vector <uint8_t> const & clauses = std::vector <uint8_t> (), bool timing = true, uint32_t pool = 30, uint8_t periods = 3, uint32_t period_length = 30);
		void send (boost::asio::ip::tcp::socket & socket);
		static int tm_to_pl_species (Species id);
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

}
}
#endif
