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

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include "inmessage.hpp"
#include "../client.hpp"

namespace technicalmachine {
namespace ps {

class Client : public ::technicalmachine::Client {
public:
	explicit Client(unsigned depth);
	void run();
	void handle_message(InMessage const & message);
	void send_channel_message(std::string const & channel, std::string const & message);
	void send_channel_message(uint32_t channel_id, std::string const & message);
	void send_private_message(std::string const & user, std::string const & message);
private:
	using Base = ::technicalmachine::Client;
	void log_in();
	void load_settings(bool reloading);
	void send_battle_challenge(std::string const & opponent) override;
	void handle_finalize_challenge(std::string const & opponent, bool accepted, bool unused = false) override;
	void join_channel(std::string const & channel);
	void part_channel(std::string const & channel);
	
	using Socket = websocketpp::client<websocketpp::config::asio_client>;
	Socket socket;
	websocketpp::connection_hdl m_handle;
	
	std::string m_host;
	std::string m_username;
	std::string m_password;
	std::vector<std::string> m_highlights;
	std::vector<std::string> m_trusted_users;
	unsigned m_chattiness;
};

}	// namespace ps
}	// namespace technicalmachine
#endif	// POKEMON_SHOWDOWN__CONNECT_HPP_
