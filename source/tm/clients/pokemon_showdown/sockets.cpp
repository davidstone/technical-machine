// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

export module tm.clients.ps.sockets;

import tm.boost_networking;

import containers;
import std_module;

namespace technicalmachine::ps {

using namespace std::string_view_literals;

namespace ssl = boost::asio::ssl;

auto make_ssl_context() -> ssl::context {
    auto context = ssl::context(ssl::context::tlsv13_client);
	context.set_default_verify_paths();
	return context;
}

export struct Sockets {
	Sockets(std::string_view const host, std::string_view const port, std::string_view const resource):
		m_ssl(make_ssl_context()),
		m_websocket(m_io, m_ssl)
	{
		connect_ssl_socket(m_io, m_websocket.next_layer(), host, port);
		m_websocket.handshake(
			std::string_view(containers::concatenate<containers::string>(host, ":"sv, port)),
			resource
		);
	}

	Sockets(Sockets &&) = delete;

	auto read_message() -> std::string_view {
		m_buffer.consume(static_cast<std::size_t>(-1));
		websocket_read(m_websocket, m_buffer);

		auto const asio_buffer = m_buffer.data();
		auto const sv = std::string_view(static_cast<char const *>(asio_buffer.data()), asio_buffer.size());

		return sv;
	}

	auto write_message(std::string_view const message) -> void {
		websocket_write(m_websocket, message);
	}

	auto authenticate(
		std::string_view const host,
		std::string_view const port,
		http::request<http::string_body> const & request
) -> http::response<http::string_body> {
		auto socket = ssl::stream<tcp::socket>(m_io, m_ssl);
		connect_ssl_socket(m_io, socket, host, port);

		http::write(socket, request);

		auto buffer = boost::beast::flat_buffer();
		auto response = http::response<http::string_body>();
		http::read(socket, buffer, response);
		return response;
	}

private:
	boost::beast::flat_buffer m_buffer;
	boost::asio::io_context m_io;
    ssl::context m_ssl;
	Websocket m_websocket;
};

} // namespace technicalmachine::ps
