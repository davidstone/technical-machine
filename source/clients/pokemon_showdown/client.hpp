// Connect to Pokemon Showdown
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

#ifndef POKEMON_SHOWDOWN__CONNECT_HPP_
#define POKEMON_SHOWDOWN__CONNECT_HPP_

#include <cstdint>
#include <string>

#include "inmessage.hpp"
#include "../network/client.hpp"

namespace technicalmachine {
namespace ps {

class Client : public network::Client {
public:
	explicit Client(unsigned set_depth);
	void run();
	void handle_message(InMessage::Message code, InMessage & msg);
	void send_channel_message(std::string const & channel, std::string const & message) override;
	void send_channel_message(uint32_t channel_id, std::string const & message) override;
	void send_private_message(std::string const & user, std::string const & message) override;
private:
	void log_in();
	void send_battle_challenge(std::string const & opponent) override;
	void handle_finalize_challenge(std::string const & opponent, bool accepted, bool unused = false) override;
	void send_keep_alive_message() override;
	void join_channel(std::string const & channel) override;
	void part_channel(std::string const & channel) override;
};

}	// namespace ps
}	// namespace technicalmachine
#endif	// POKEMON_SHOWDOWN__CONNECT_HPP_
