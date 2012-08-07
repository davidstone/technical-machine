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

#ifndef NETWORK__INMESSAGE_HPP_
#define NETWORK__INMESSAGE_HPP_

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <boost/asio/ip/tcp.hpp>

namespace technicalmachine {
namespace network {
class GenericClient;

class InMessage {
	public:
		std::vector <uint8_t> buffer;
		size_t index;
	public:
		InMessage ();
		virtual ~InMessage() {}
		void reset (size_t bytes);
		uint8_t read_byte ();
		uint16_t read_short ();
		uint32_t read_int ();
		void read_header(boost::asio::ip::tcp::socket & socket, GenericClient * client);
	private:
		uint32_t read_bytes (size_t bytes);
		virtual size_t header_size() const = 0;
		virtual void read_body (boost::asio::ip::tcp::socket & socket, GenericClient * client) = 0;
};

}	// namespace network
}	// namespace technicalmachine
#endif	// NETWORK__INMESSAGE_HPP_
