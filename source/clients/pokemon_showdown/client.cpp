// Connect to Pokemon Showdown
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

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

#include "client.hpp"

#include "battle.hpp"
#include "inmessage.hpp"
#include "outmessage.hpp"

#include "../../settings_file.hpp"
#include "../../team.hpp"

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <string>

namespace technicalmachine {
namespace ps {

namespace {
template<typename Container>
Container split(std::string const & str, char delimiter) {
	Container container;
	auto previous = begin(str);
	while (true) {
		auto const found = std::find(previous, end(str), delimiter);
		container.emplace_back(previous, found);
		if (found == end(str)) {
			break;
		}
		previous = std::next(found);
	}
	return container;
}

using message_ptr = websocketpp::config::asio_client::message_type::ptr;

InMessage parse_message(message_ptr const & ptr) {
	static constexpr char delimiter = '|';
	auto tokens = split<containers::vector<std::string>>(ptr->get_payload(), delimiter);
	assert(size(tokens) >= 2_bi);
	std::string & room = *begin(tokens);
	std::string & type = *(begin(tokens) + 1_bi);
	InMessage::Data data(std::make_move_iterator(begin(tokens) + 2_bi), std::make_move_iterator(end(tokens)));
	return InMessage(std::move(room), std::move(type), std::move(data));
}
}	// namespace

Client::Client(unsigned depth):
	::technicalmachine::Client(depth) {
	load_settings (false);
	while (m_username.empty()) {
		std::cerr << "Add a username and password entry to " << Settings::file_name() << " and hit enter.";
		std::cin.get();
		load_settings(false);
	}
	socket.init_asio();
	socket.set_message_handler([&](websocketpp::connection_hdl, message_ptr message) {
		handle_message(parse_message(message));
		std::cout << "Waiting for next message.\n";
	});
	log_in();
}

void Client::log_in() {
#if 0
	websocketpp::lib::error_code ec;
	auto connection_pointer = socket.get_connection(m_host, ec);
	m_handle = connection_pointer->get_handle();
	socket.connect(std::move(connection_pointer));
#endif
}

// Temporary copy and paste here until I implement my better structure for
// managing clients.
void Client::load_settings(bool const reloading) {
	auto settings = reloading ? Base::load_settings(true) : Settings();
	m_chattiness = settings.chattiness;
	
	if (!reloading) {
		Server & server = front(settings.servers);
		m_host = server.host;
		m_username = server.username;
		if (server.password.empty()) {
			server.password = random_string(31);
			settings.write();
		}
		m_password = server.password;
	}
}

void Client::run() {
	socket.run();
}

void Client::handle_message(InMessage const & message) {
	if (message.type() == "challstr") {
		// auto const & key_id = message.at(0);
		// auto const & challenge = message.at(1);
		websocketpp::lib::error_code ec;
		std::string const request = "|/join lobby";
		socket.send(m_handle, request, websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cerr << ec.message() << '\n';
		}
	}
	else if (message.type() == "popup") {
		for (auto const & text : message) {
			std::cout << text << '\n';
		}
	}
	else {
		for (auto const & text : message) {
			std::cout << text << '|';
		}
		std::cout << "\nPlease type in a command.\n";
		std::string request;
		std::getline(std::cin, request);
		if (request.empty()) {
			return;
		}
		websocketpp::lib::error_code ec;
		socket.send(m_handle, request, websocketpp::frame::opcode::text, ec);
		if (ec) {
			std::cerr << ec.message() << '\n';
		}
	}
}

void Client::send_battle_challenge(std::string const & opponent) {
	static_cast<void>(opponent);
}

void Client::handle_finalize_challenge(std::string const & opponent, bool accepted, bool) {
	static_cast<void>(opponent);
	static_cast<void>(accepted);
}

void Client::join_channel(std::string const & channel) {
	static_cast<void>(channel);
}

void Client::part_channel(std::string const & channel) {
	static_cast<void>(channel);
}

void Client::send_channel_message(std::string const & channel, std::string const & message) {
	static_cast<void>(channel);
	static_cast<void>(message);
}

void Client::send_channel_message(uint32_t channel_id, std::string const & message) {
	static_cast<void>(channel_id);
	static_cast<void>(message);
}

void Client::send_private_message(std::string const & user, std::string const & message) {
	static_cast<void>(user);
	static_cast<void>(message);
}

}	// namespace ps
}	// namespace technicalmachine
