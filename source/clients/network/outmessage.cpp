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

#include "../../cryptography/byte_order.hpp"

namespace technicalmachine {
namespace network {

OutMessage::OutMessage (uint8_t const code) {
	write_byte (code);
}

template<typename Integer>
void OutMessage::write_bytes(Integer const bytes) {
	Integer const network_byte = boost::endian::h_to_n(bytes);
	uint8_t const * byte = reinterpret_cast<uint8_t const *>(&network_byte);
	for (unsigned n = 0; n != sizeof(Integer); ++n)
		buffer.emplace_back(*(byte + n));
}

void OutMessage::write_byte (uint8_t byte) {
	write_bytes(byte);
}

void OutMessage::write_short (uint16_t bytes) {
	write_bytes(bytes);
}

void OutMessage::write_int (uint32_t bytes) {
	write_bytes(bytes);
}

void OutMessage::send(boost::asio::ip::tcp::socket & socket) {
	finalize();
	boost::asio::write(socket, boost::asio::buffer(buffer));
}

}	// namespace technicalmachine
}	// namespace network
