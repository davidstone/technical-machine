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

InMessage::InMessage(Room room_, Type type_, Data data_):
	m_room(std::move(room_)),
	m_type(std::move(type_)),
	m_data(std::move(data_)) {
}

InMessage::Type const & InMessage::type() const noexcept {
	return m_type;
}

std::string const & InMessage::at(Data::size_type position) const {
	return m_data.at(position);
}

InMessage::Data::const_iterator InMessage::begin() const {
	return m_data.begin();
}
InMessage::Data::const_iterator InMessage::end() const {
	return m_data.end();
}


}	// namespace ps
}	// namespace technicalmachine
