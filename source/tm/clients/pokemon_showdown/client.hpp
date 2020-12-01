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
#include <tm/clients/pokemon_showdown/to_packed_format.hpp>

#include <tm/evaluate/evaluate.hpp>

#include <tm/team_predictor/team_predictor.hpp>
#include <tm/team_predictor/usage_stats.hpp>

#include <tm/load_team_from_file.hpp>
#include <tm/settings_file.hpp>

#include <containers/trivial_inplace_function.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>

#include <unordered_set>
#include <random>
#include <string>
#include <string_view>

namespace technicalmachine {
namespace ps {
namespace http = boost::beast::http;

struct ClientImpl {
	using AuthenticationSignature = http::response<http::string_body>(
		std::string_view,
		std::string_view,
		http::request<http::string_body> const &
	);
	using AuthenticationFunction = containers::trivial_inplace_function<
		AuthenticationSignature,
		sizeof(void *)
	>;
	ClientImpl(SettingsFile, DepthValues, SendMessageFunction, AuthenticationFunction);
	void run(DelimitedBufferView<std::string_view> messages);

private:
	template<Generation generation>
	auto generate_team() -> Team<generation> {
		return m_settings.team_file ?
			load_team_from_file<generation>(m_random_engine, *m_settings.team_file) :
			::technicalmachine::generate_team<generation>(
				m_all_usage_stats[generation],
				use_lead_stats,
				m_random_engine
			);
	}

	
	void send_team(Generation);

	void handle_message(InMessage message);
	void send_channel_message(std::string const & channel, std::string const & message);
	void send_private_message(std::string const & user, std::string const & message);

	void join_channel(std::string const & channel);
	
	void authenticate(std::string_view challstr);
	
	DelimitedBufferView<std::string_view> read_message();

	std::random_device m_rd;
	std::mt19937 m_random_engine;

	AllUsageStats m_all_usage_stats;
	AllEvaluate m_evaluate;

	SettingsFile m_settings;
	
	JSONParser m_parse_json;
	
	std::unordered_set<std::string> m_trusted_users;
	DepthValues m_depth;
	
	Battles m_battles;
	
	SendMessageFunction m_send_message;
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
