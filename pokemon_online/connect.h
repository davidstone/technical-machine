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

#ifndef POKEMON_ONLINE_CONNECT_H_
#define POKEMON_ONLINE_CONNECT_H_

#include <cstdint>
#include <map>
#include <string>

// #include "battle.h"
#include "../network/connect.h"

namespace technicalmachine {
namespace po {

class Channel;
class Metagame;

class BotClient : public network::GenericClient {
	private:
//		std::map <std::string, uint32_t> channels;
//		std::map <std::string, Battle> challenges;		// Battles that have not yet begun
//		std::map <uint8_t, Battle> battles;			// Battles currently underway
	public:
		explicit BotClient (int depth_);
	private:
		void log_in ();
//		void authenticate ();
	public:
		void run ();
	private:
		void send_keep_alive_message ();
};

}
}
#endif // POKEMON_ONLINE_CONNECT_HPP_
