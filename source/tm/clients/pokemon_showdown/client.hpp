// Connect to Pokemon Showdown
// Copyright (C) 2019 David Stone
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

#include <tm/clients/pokemon_showdown/battles.hpp>
#include <tm/clients/pokemon_showdown/inmessage.hpp>
#include <tm/clients/pokemon_showdown/json_parser.hpp>

#include <tm/evaluate/evaluate.hpp>
#include <tm/team_predictor/usage_stats.hpp>
#include <tm/settings_file.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <functional>
#include <unordered_set>
#include <random>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {
namespace http = boost::beast::http;

struct ClientImpl {
	using AuthenticationFunction = std::function<
		http::response<http::string_body>(std::string_view, std::string_view, http::request<http::string_body> const &)
	>;
	ClientImpl(SettingsFile, DepthValues, BattleParser::SendMessageFunction, AuthenticationFunction);
	void run(DelimitedBufferView<std::string_view> messages);

private:
	auto generate_team(Generation const generation) -> Team;
	
	void send_team(Generation const generation);

	void handle_message(InMessage message);
	void send_channel_message(std::string const & channel, std::string const & message);
	void send_private_message(std::string const & user, std::string const & message);

	void join_channel(std::string const & channel);
	
	void authenticate(std::string_view challstr);
	
	DelimitedBufferView<std::string_view> read_message();

	std::random_device m_rd;
	std::mt19937 m_random_engine;

	AllUsageStats m_all_usage_stats;
	Evaluate m_evaluate;

	SettingsFile m_settings;
	
	JSONParser m_parse_json;
	
	std::unordered_set<std::string> m_trusted_users;
	DepthValues m_depth;
	
	Battles m_battles;
	
	BattleParser::SendMessageFunction m_send_message;
	AuthenticationFunction m_authenticate;
};

struct Client {
	Client(SettingsFile settings, DepthValues);
	[[noreturn]] void run();
private:
	struct Sockets {
		using tcp = boost::asio::ip::tcp;

		Sockets(std::string_view host, std::string_view port, std::string_view resource);
		Sockets(Sockets &&) = delete;

		auto read_message() -> DelimitedBufferView<std::string_view>;
		void write_message(std::string_view message);
		auto authenticate(std::string_view host, std::string_view port, http::request<http::string_body> const & request) -> http::response<http::string_body>;
	
	private:
		auto make_connected_socket(std::string_view host, std::string_view port) -> tcp::socket;
		boost::beast::flat_buffer m_buffer;
		boost::asio::io_service m_io;
		tcp::socket m_socket;
		boost::beast::websocket::stream<tcp::socket &> m_websocket;
	};
	
	Sockets m_sockets;
	ClientImpl m_impl;
};

}	// namespace ps
}	// namespace technicalmachine
