// Generic outgoing messages
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

#include "outmessage.hpp"

#include <cstdint>

#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <endian/endian.hpp>

namespace technicalmachine {
namespace network {

OutMessage::OutMessage (uint8_t const code) {
	write_byte (code);
}

namespace {

template<typename Integer>
void write_bytes(std::vector<uint8_t> & buffer, Integer const bytes) {
	Integer const network_byte = boost::endian::h_to_n(bytes);
	uint8_t const * byte = reinterpret_cast<uint8_t const *>(&network_byte);
	for (unsigned n = 0; n != sizeof(Integer); ++n)
		buffer.emplace_back(*(byte + n));
}

}	// namespace

void OutMessage::write_byte (uint8_t byte) {
	write_bytes(buffer, byte);
}

void OutMessage::write_short (uint16_t bytes) {
	write_bytes(buffer, bytes);
}

void OutMessage::write_int (uint32_t bytes) {
	write_bytes(buffer, bytes);
}

void OutMessage::send(boost::asio::ip::tcp::socket & socket) {
	finalize();
	boost::asio::write(socket, boost::asio::buffer(buffer));
}

}	// namespace technicalmachine
}	// namespace network
