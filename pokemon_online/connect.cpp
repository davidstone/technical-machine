// Connect to Pokemon Online
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
#include <map>
#include <string>

#include "outmessage.h"

namespace technicalmachine {
namespace po {

BotClient::BotClient (int depth_) : network::GenericClient (depth_) {
	log_in ();
}

void BotClient::log_in () {
	OutMessage message (OutMessage::LOG_IN);
	message.write_string (username);
	message.send (socket);
}

void BotClient::run () {
	unsigned const timer_length = 45;
	reset_timer (timer_length);

/*	InMessage msg;
	msg.read_header (socket, this);
*/
	io.run();
}

void BotClient::send_keep_alive_message () {
	OutMessage msg (OutMessage::KEEP_ALIVE);
	msg.send (socket);
}


}		// namespace po
}		// namespace technicalmachine
