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

#include <cstdint>
#include <vector>
#include <boost/asio.hpp>

#include "outmessage.h"

namespace technicalmachine {
namespace pl {

OutMessage::OutMessage (uint8_t code) {
	buffer.push_back (code);
}

void OutMessage::write_byte (uint8_t byte) {
	buffer.push_back (byte);
}

void OutMessage::write_short (uint16_t bytes) {
	uint16_t network_byte = htons (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint16_t); ++n)
		buffer.push_back (*(byte + n));
}

void OutMessage::write_int (uint32_t bytes) {
	uint32_t network_byte = htonl (bytes);
	uint8_t * byte = reinterpret_cast <uint8_t *> (&network_byte);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.push_back (*(byte + n));
}

void OutMessage::write_string (std::string const & string) {
	write_short (string.length());
	for (std::string::const_iterator it = string.begin(); it != string.end(); ++it)
		buffer.push_back (*it);
}

void OutMessage::finalize (boost::asio::ip::tcp::socket & socket) {
	uint32_t length = buffer.size() - 1;
	uint8_t * byte = reinterpret_cast <uint8_t *> (&length);
	for (unsigned n = 0; n != sizeof (uint32_t); ++n)
		buffer.insert (buffer.begin() + 1, *(byte + n));
	boost::asio::write (socket, boost::asio::buffer (buffer));
}

}
}
