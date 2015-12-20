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

#pragma once

#include <bounded/integer.hpp>

#include <containers/vector/vector.hpp>

#include <boost/asio/ip/tcp.hpp>

#include <cstddef>
#include <cstdint>
#include <string>

namespace technicalmachine {
using namespace bounded::literal;

namespace network {
struct Client;

struct InMessage {
	InMessage ();
	virtual ~InMessage() = default;

	template<typename Size>
	void reset(Size bytes) {
		clear(buffer);
		resize(buffer, bytes);
		index = 0_bi;
	}

	uint8_t read_byte ();
	uint16_t read_short ();
	uint32_t read_int ();
	void read_header(boost::asio::ip::tcp::socket & socket, Client & client);
	void read_remaining_bytes();	// helpful for debugging
private:
	using Buffer = containers::vector<std::uint8_t>;
	uint32_t read_bytes(Buffer::size_type bytes);
	virtual size_t header_size() const = 0;
	virtual void read_body (boost::asio::ip::tcp::socket & socket, Client & client) = 0;
public:
	Buffer buffer;
	Buffer::size_type index;
};

}	// namespace network
}	// namespace technicalmachine
