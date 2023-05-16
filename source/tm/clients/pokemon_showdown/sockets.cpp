// Copyright David Stone 2020.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// See https://github.com/Zarel/Pokemon-Showdown/blob/master/protocol-doc.md
// for the full protocol.

module;

#include <std_module/prelude.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

export module tm.clients.ps.sockets;

import std_module;

namespace technicalmachine::ps {
namespace http = boost::beast::http;
using tcp = boost::asio::ip::tcp;

export struct Sockets {
	Sockets(std::string_view const host, std::string_view const port, std::string_view const resource):
		m_socket(make_connected_socket(host, port)),
		m_websocket(m_socket)
	{
		m_websocket.handshake(host, resource);
	}

	Sockets(Sockets &&) = delete;

	auto read_message() -> std::string_view {
		m_buffer.consume(static_cast<std::size_t>(-1));
		m_websocket.read(m_buffer);

		auto const asio_buffer = m_buffer.data();
		auto const sv = std::string_view(static_cast<char const *>(asio_buffer.data()), asio_buffer.size());

		return sv;
	}

	auto write_message(std::string_view const message) -> void {
		m_websocket.write(boost::asio::buffer(message));
	}

	auto authenticate(std::string_view const host, std::string_view const port, http::request<http::string_body> const & request) -> http::response<http::string_body> {
		auto socket = make_connected_socket(host, port);

		http::write(socket, request);

		auto buffer = boost::beast::flat_buffer{};
		auto response = http::response<http::string_body>{};
		http::read(socket, buffer, response);
		return response;
	}

private:
	auto make_connected_socket(std::string_view const host, std::string_view const port) -> tcp::socket {
		auto socket = tcp::socket(m_io);
		auto resolver = tcp::resolver(m_io);
		boost::asio::connect(socket, resolver.resolve(host, port));
		return socket;
	}

	boost::beast::flat_buffer m_buffer;
	boost::asio::io_context m_io;
	tcp::socket m_socket;
	boost::beast::websocket::stream<tcp::socket &> m_websocket;
};

} // namespace technicalmachine::ps
