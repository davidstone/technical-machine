// Generic incoming messages
// Copyright (C) 2015 David Stone
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

#include "buffer_overrun.hpp"

#include <bounded/integer_range.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>

#include <cstddef>
#include <cstdint>
#include <iostream>

namespace technicalmachine {
namespace network {

InMessage::InMessage ():
	index(0_bi) {
}

uint32_t InMessage::read_bytes(Buffer::size_type bytes) {
	// Verify that I actually have enough room in my buffer to read that many
	// bytes.
	if (bytes > size(buffer) - index) {
		throw BufferOverrun();
	}
	uint32_t data = 0;
	// I have to load byte-by-byte and use shifts and addition instead of
	// just loading the entire chunk of memory and interpreting it as a
	// uint32_t because doing otherwise would break alignment requirements
	// for memory access.
	for (auto const n : bounded::integer_range(bytes)) {
		data += static_cast<uint32_t>(at(buffer, index, bounded::non_check)) << static_cast<uint32_t>(8_bi * (bytes - n - 1_bi));
		++index;
	}
	return data;
}

uint8_t InMessage::read_byte () {
	return static_cast<uint8_t>(read_bytes(1_bi));
}

uint16_t InMessage::read_short () {
	return static_cast<uint16_t>(read_bytes(2_bi));
}

uint32_t InMessage::read_int () {
	return read_bytes(4_bi);
}

void InMessage::read_header(boost::asio::ip::tcp::socket & socket, Client & client) {
	reset(header_size());
	// TODO: handle errors
	boost::asio::async_read(socket, boost::asio::buffer(data(buffer), static_cast<std::size_t>(size(buffer))), [&](auto, auto){ this->read_body(socket, client); });
}

void InMessage::read_remaining_bytes() {
	auto const backup = index;
	std::cerr << "As int:\n";
	while (index != size(buffer)) {
		std::cerr << '\t' << static_cast<int>(read_byte()) << '\n';
	}
	index = backup;
	std::cerr << "As char:\n\t";
	while (index != size(buffer)) {
		std::cerr << static_cast<char>(read_byte());
	}
	std::cerr << '\n';
}

}	// namespace network
}	// namespace technicalmachine
