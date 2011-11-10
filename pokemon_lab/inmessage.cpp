// Pokemon Lab incoming messages
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

#include "inmessage.h"
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "../species.h"
#include "connect.h"

namespace technicalmachine {
namespace pl {

InMessage::InMessage (): network::InMessage::InMessage () {
}

std::string InMessage::read_string () {
	uint16_t length = read_short ();
	std::string data = "";
	for (uint16_t n = 0; n != length; ++n)
		data += read_byte ();
	return data;
}

void InMessage::read_header (boost::asio::ip::tcp::socket & socket, Client * client) {
	reset (5);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& InMessage::read_body, this, boost::ref (socket), client));
}

void InMessage::read_body (boost::asio::ip::tcp::socket & socket, Client * client) {
	// extract the message type and length components
	Message code = static_cast <InMessage::Message> (read_byte ());
	uint32_t bytes = read_int ();

	reset (bytes);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& Client::handle_message, client, code, boost::ref (*this)));
}

} // namespace pl
} // namespace technicalmachine
