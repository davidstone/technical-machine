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

#ifndef POKEMON_SHOWDOWN__INMESSAGE_HPP_
#define POKEMON_SHOWDOWN__INMESSAGE_HPP_

#include <string>
#include "../network/inmessage.hpp"

namespace technicalmachine {
namespace network {
class Client;
}	// namespace network
namespace ps {

class InMessage : public network::InMessage::InMessage {
	public:
		std::string read_string();
		void read_body(boost::asio::ip::tcp::socket & socket, network::Client * client) override;
		enum Message {
		};
	private:
		size_t header_size() const override;
};

}	// namespace ps
}	// namespace technicalmachine
#endif	// POKEMON_SHOWDOWN__INMESSAGE_HPP_
