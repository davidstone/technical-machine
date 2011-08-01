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

#include <cstdint>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "inmessage.h"
#include "connect.h"

namespace technicalmachine {
namespace pl {

InMessage::InMessage ():
	index (0) {
}

void InMessage::reset (unsigned bytes) {
	buffer.clear();
	buffer.resize (bytes);
	index = 0;
}

void InMessage::read_header (boost::asio::ip::tcp::socket & socket, BotClient * client) {
	reset (5);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& InMessage::read_body, this, socket, client));
}

void InMessage::read_body (boost::asio::ip::tcp::socket & socket, BotClient * client) {
	// extract the message type and length components
	Message code = static_cast <InMessage::Message> (read_byte ());
	uint32_t bytes = read_int ();

	reset (bytes);
	boost::asio::async_read (socket, boost::asio::buffer (buffer), boost::bind (& BotClient::handle_message, client, code, *this));
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
	std::string data = "";
	for (unsigned n = 0; n != length; ++n) {
		data += buffer [index];
		++index;
	}
	return data;
}

void InMessage::skip () {
	buffer.clear();
}

}
}
