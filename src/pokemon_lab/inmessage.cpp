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

#include "inmessage.hpp"

#include <cstdint>
#include <string>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>

#include "connect.hpp"

namespace technicalmachine {
namespace network {
class GenericClient;
}	// namespace network
namespace pl {

InMessage::InMessage ():
	network::InMessage::InMessage () {
}

std::string InMessage::read_string () {
	uint16_t length = read_short ();
	std::string data = "";
	for (uint16_t n = 0; n != length; ++n)
		data += static_cast<char> (read_byte ());
	return data;
}

void InMessage::read_body (boost::asio::ip::tcp::socket & socket, network::GenericClient * client) {
	// extract the message type and length components
	// TODO: store in the underlying type instead of the enum so the compiler
	// can assume all enums are within their range.
	Message code = static_cast <InMessage::Message> (read_byte ());
	uint32_t bytes = read_int ();

	reset (bytes);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& Client::handle_message, static_cast<Client *>(client), code, boost::ref (*this)));
}

}	// namespace pl
}	// namespace technicalmachine
