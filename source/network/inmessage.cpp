// Generic incoming messages
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

#include <cstddef>
#include <cstdint>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>

#include "buffer_overrun.hpp"

namespace technicalmachine {
namespace network {

InMessage::InMessage ():
	index (0) {
}

void InMessage::reset (size_t bytes) {
	buffer.clear();
	buffer.resize (bytes);
	index = 0;
}

uint32_t InMessage::read_bytes (size_t bytes) {
	// Verify that I actually have enough room in my buffer to read that many
	// bytes. index is guaranteed to be between 0 and buffer.size() inclusive,
	// so there is no risk of overflow in the comparison.
	if (buffer.size() - index >= bytes) {
		uint32_t data = 0;
		// I have to load byte-by-byte and use shifts and addition instead of
		// just loading the entire chunk of memory and interpreting it as a
		// uint32_t because doing otherwise would break alignment requirements
		// for memory access.
		for (size_t n = 0; n != bytes; ++n) {
			data += static_cast<uint32_t> (buffer [index]) << (8 * (bytes - n - 1));
			++index;
		}
		return data;
	}
	else {
		throw BufferOverrun ();
	}
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

void InMessage::read_header(boost::asio::ip::tcp::socket & socket, GenericClient * client) {
	reset(header_size());
	boost::asio::async_read(socket, boost::asio::buffer(buffer), boost::bind(& InMessage::read_body, this, boost::ref(socket), client));
}

}	// namespace network
}	// namespace technicalmachine
