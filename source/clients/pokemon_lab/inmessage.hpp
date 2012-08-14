// Pokemon Lab incoming messages
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

#ifndef POKEMON_LAB__INMESSAGE_HPP_
#define POKEMON_LAB__INMESSAGE_HPP_

#include <string>
#include "../network/inmessage.hpp"

namespace technicalmachine {
namespace network {
class GenericClient;
}	// namespace network
namespace pl {


class InMessage : public network::InMessage::InMessage {
	public:
		InMessage ();
		std::string read_string ();
		void read_body (boost::asio::ip::tcp::socket & socket, network::GenericClient * client);

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
			BATTLE_BEGIN = 12,
			REQUEST_ACTION = 13,
			BATTLE_POKEMON = 14,
			BATTLE_PRINT = 15,
			BATTLE_END = 16,	// "BATTLE_VICTORY"
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
		enum PrintCategory {
			TYPES = 0,
			NATURES = 1,
			STATS = 2,
			STATS_LONG = 3,
			BATTLE_MESSAGES = 4,
			STATUS_EFFECTS_STAT_LEVEL = 5,
			STATUS_EFFECTS_BURN = 6,
			STATUS_EFFECTS_CONFUSION = 7,
			STATUS_EFFECTS_FREEZE = 8,
			STATUS_EFFECTS_PARALYSIS = 9,
			STATUS_EFFECTS_POISON = 10,
			STATUS_EFFECTS_SLEEP = 11,
			STATUS_EFFECTS_FLINCH = 12,
			STATUS_EFFECTS_STOCKPILE = 13,
			STATUS_EFFECTS_LEECH_SEED = 14,
			STATUS_EFFECTS_ATTRACT = 15,
			STATUS_EFFECTS_SPIKES = 16,
			WEATHER_FOG = 17,
			WEATHER_HAIL = 18,
			WEATHER_RAIN = 19,
			WEATHER_SANDSTORM = 20,
			WEATHER_SUN = 21,
			BATTLE_MESSAGES_UNIQUE = 22,
			BATTLE_MESSAGES_CHARGE = 23,
			ABILITIES = 24,
			ABILITY_MESSAGES = 25,
			ITEM_MESSAGES = 26,
			CLIENT_MESSAGES = 27
		};
	private:
		size_t header_size() const { return 5; }
};

}	// namespace pl
}	// namespace technicalmachine
#endif	// POKEMON_LAB__INMESSAGE_HPP_
