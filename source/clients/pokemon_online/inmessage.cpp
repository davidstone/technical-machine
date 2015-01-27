// Pokemon Online incoming messages
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

#include "client.hpp"
#include "../network/invalid_packet.hpp"

#include <boost/asio/buffer.hpp>
#include <boost/asio/read.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/bind.hpp>

#include <cstdint>
#include <string>

namespace technicalmachine {
namespace po {

InMessage::InMessage (): network::InMessage::InMessage () {
}

std::string InMessage::read_string () {
	std::string data = "";
	uint32_t const bytes = read_int ();
	// QString reports a size of 0xFFFFFFFF if the string is null. I'll just call it empty.
	if (bytes != 0xFFFFFFFF) {
		uint32_t const number_of_utf16_characters = bytes / 2;
		for (uint32_t n = 0; n != number_of_utf16_characters; ++n) {
			data += static_cast <char> (read_short ());
		}
	}
	return data;
}

void InMessage::read_body(boost::asio::ip::tcp::socket & socket, network::Client & client) {
	// extract the message type and length components
	auto const bytes = read_short();
	// Don't do an invalid call to new if the server says the message has a length of 0
	if (bytes > 0) {
		auto const code = static_cast<Message>(read_byte());
		reset(bytes - 1u);
		// TODO: handle errors
		boost::asio::async_read(socket, boost::asio::buffer(buffer), [&](auto, auto){
			static_cast<Client &>(client).handle_message(code, *this);
		});
	} else {
		throw technicalmachine::network::InvalidPacket ("Server sent message of length 0.");
	}
}

}	// namespace po
}	// namespace technicalmachine
