// Pokemon Showdown incoming messages
// Copyright (C) 2013 David Stone
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

#include "client.hpp"
#include "../network/invalid_packet.hpp"

namespace technicalmachine {
namespace ps {

std::string InMessage::read_string() {
	throw std::runtime_error("Unimplemented read string");
}

void InMessage::read_body(boost::asio::ip::tcp::socket & socket, network::Client * client) {
}

size_t InMessage::header_size() const {
	throw std::runtime_error("Unimplemented header size");
}

}	// namespace ps
}	// namespace technicalmachine
