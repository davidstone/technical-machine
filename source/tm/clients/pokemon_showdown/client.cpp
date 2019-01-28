// Connect to Pokemon Showdown
// Copyright (C) 2018 David Stone
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

#include <tm/clients/pokemon_showdown/client.hpp>

#include <tm/clients/pokemon_showdown/inmessage.hpp>

#include <tm/clients/random_string.hpp>
#include <tm/settings_file.hpp>
#include <tm/team.hpp>

#include <containers/scope_guard.hpp>

#include <boost/asio/connect.hpp>
#include <boost/beast/http.hpp>

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace technicalmachine {
namespace ps {
namespace {

auto load_lines_from_file(std::filesystem::path const & file_name) {
	auto container = std::unordered_set<std::string>{};
	auto file = std::ifstream(file_name);
	auto line = std::string{};
	while (getline(file, line)) {
		if (!line.empty())
			container.insert(std::move(line));
	}
	return container;
}

}	// namespace

Client::Sockets::Sockets(std::string_view const host, std::string_view const port, std::string_view const resource):
	m_socket(make_connected_socket(host, port)),
	m_websocket(m_socket)
{
	m_websocket.handshake(
		boost::string_view(host.data(), host.size()),
		boost::string_view(resource.data(), resource.size())
	);
}

auto Client::Sockets::make_connected_socket(std::string_view const host, std::string_view const port) -> tcp::socket {
	auto socket = tcp::socket(m_io);
	auto resolver = tcp::resolver(m_io);
	boost::asio::connect(socket, resolver.resolve(host, port));
	return socket;
}

auto Client::Sockets::read_message() -> BufferView<char> {
	m_buffer.consume(static_cast<std::size_t>(-1));
	m_websocket.read(m_buffer);

	auto const asio_buffer = m_buffer.data();
	auto const sv = std::string_view(static_cast<char const *>(asio_buffer.data()), asio_buffer.size());

	return BufferView(sv, '\n');
}

void Client::Sockets::write_message(std::string_view const message) {
	m_websocket.write(boost::asio::buffer(message));
}


Client::Client(SettingsFile settings, unsigned depth):
	m_random_engine(m_rd()),
	m_usage_stats("settings/4/OU"),
	m_settings(std::move(settings)),
	m_trusted_users(load_lines_from_file("settings/trusted_users.txt")),
	m_depth(depth),
	m_sockets(m_settings.host, m_settings.port, m_settings.resource)
{
	std::cout << "Connected\n";
}

void Client::run() {
	std::cout << "Running\n";
	while (true) {
		auto messages = m_sockets.read_message();
		auto const has_room = !messages.remainder().empty() and messages.remainder().front() == '>';
		auto const room = has_room ? messages.next().substr(1) : std::string_view{};
		while (!messages.remainder().empty()) {
			auto const next = messages.next();
			auto print_on_exception = containers::scope_guard([=]{ std::cerr << next << '\n'; });
			handle_message(InMessage(room, BufferView(next, '|')));
			print_on_exception.dismiss();
		}
	}
}

void Client::handle_message(InMessage message) {
	if (message.type() == ":") {
		// message.remainder() == seconds since 1970
	} else if (message.type() == "b" or message.type() == "B" or message.type() == "battle") {
		// message.remainder() == ROOMID|username|username
	} else if (message.type() == "c" or message.type() == "chat") {
		// message.remainder() == username|message
	} else if (message.type() == "c:") {
		// message.remainder() == seconds since 1970|username|message
	} else if (message.type() == "challstr") {
		authenticate(message.remainder());
		// After logging in, send "|/search FORMAT_NAME" to begin laddering
	} else if (message.type() == "formats") {
		// message.remainder() == | separated list of formats with special rules
	} else if (message.type() == "html") {
		// message.remainder() == HTML
	} else if (message.type() == "init") {
		if (message.next() == "battle") {
			m_battles.add_pending(
				std::string(message.room()),
				m_settings.username,
				m_usage_stats,
				m_evaluate,
				m_depth,
				std::mt19937(m_rd()),
				generate_team()
			);
		}
	} else if (message.type() == "j" or message.type() == "J" or message.type() == "join") {
		// message.remainder() == username
	} else if (message.type() == "l" or message.type() == "L" or message.type() == "leave") {
		// message.remainder() == username
	} else if (message.type() == "n" or message.type() == "N" or message.type() == "name") {
		// message.remainder() == new username|old username
	} else if (message.type() == "nametaken") {
		auto const username = message.next();
		std::cerr << "Could not change username to " << username << " because: " << message.remainder() << '\n';
	} else if (message.type() == "popup") {
		std::cout << "popup message: " << message.remainder() << '\n';
	} else if (message.type() == "pm") {
		auto const from = message.next();
		auto const to = message.next();
		std::cout << "PM from " << from << " to " << to << ": " << message.remainder() << '\n';
	} else if (message.type() == "queryresponse") {
		// message.remainder() == QUERYTYPE|JSON
	} else if (message.type() == "uhtml" or message.type() == "uhtmlchange") {
		// message.remainder() == NAME|HTML
	} else if (message.type() == "updatechallenges") {
		auto const json = m_parse_json(message.remainder());
		for (auto const & challenge : json.get_child("challengesFrom")) {
			auto const is_trusted = m_trusted_users.find(challenge.first) != m_trusted_users.end();
			auto const command = is_trusted ? "|/accept " : "|/reject ";
			if (!is_trusted) {
				std::cout << "Rejected a challenge from " << challenge.first << '\n';
			}
			m_sockets.write_message(command + challenge.first);
		}
		// "cancelchallenge" is the command to stop challenging someone
	} else if (message.type() == "updateuser") {
		// message.remainder() == username|guest ? 0 : 1|AVATAR
	} else if (message.type() == "updatesearch") {
		// message.remainder() == JSON: battles you are searching for
	} else if (message.type() == "usercount") {
		// message.remainder() == number of users on server
	} else if (message.type() == "users") {
		// message.remainder() == comma separated list of users
	} else if (m_battles.handle_message(message, [&](std::string_view const output) { m_sockets.write_message(output); })) {
	} else {
		std::cout << "Received unknown message in room: " << message.room() << " type: " << message.type() << "\n\t" << message.remainder() << '\n';
	}
}

void Client::authenticate(std::string_view const challstr) {
	// In theory, if we ever support session cookies, make HTTP GET:
	// http://play.pokemonshowdown.com/action.php?act=upkeep&challstr=CHALLSTR
	// Otherwise, make HTTP POST: http://play.pokemonshowdown.com/action.php
	// with data act=login&name=USERNAME&pass=PASSWORD&challstr=CHALLSTR
	namespace http = boost::beast::http;
	constexpr auto host = "play.pokemonshowdown.com";
	auto socket = m_sockets.make_connected_socket(host, "80");
	
	constexpr auto version = 11U;
	auto request = http::request<http::string_body>{
		http::verb::post,
		"/action.php",
		version,
		("act=login&name=" + m_settings.username + "&pass=" + m_settings.password + "&challstr=").append(challstr)
	};
	request.set(http::field::host, host);
	request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
	request.set(http::field::content_type, "application/x-www-form-urlencoded");
	request.prepare_payload();

	http::write(socket, request);
	
	auto buffer = boost::beast::flat_buffer{};
	auto response = http::response<http::string_body>{};
	http::read(socket, buffer, response);
	
	// Sadly, it does not look like it is possible to use
	// boost::property_tree to directly parse JSON from some arbitrary
	// range. I could create my own custom stream class that lets me do this
	// without all of the copying and memory allocation, but it doesn't seem
	// worth it considering how rare this will be.
	//
	// Response begins with ']' followed by JSON object.
	response.body().erase(0U, 1U);
	auto const json = m_parse_json(response.body());
	m_sockets.write_message("|/trn " + m_settings.username + ",0," + json.get<std::string>("assertion"));
}

void Client::join_channel(std::string const & channel) {
	m_sockets.write_message("|/join " + channel);
}

void Client::part_channel(std::string const & channel) {
	std::cout << "part_channel\n";
	static_cast<void>(channel);
}

void Client::send_channel_message(std::string const & channel, std::string const & message) {
	m_sockets.write_message(channel + "|/msg " + message);
}

}	// namespace ps
}	// namespace technicalmachine
